/* 
 * File:   main.c
 * Author: jbera
 *
 * Created on June 8, 2019, 6:54 PM
 */
/* 
 * File:   main.c
 * Author: jbera
 *
 * Created on May 30, 2019, 8:08 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "ili9341.h"
#include "initialization.h"
/*
 * 
 */

//FROM HW5
void initExpander(){
    ANSELBbits.ANSB2 = 0;//Turn off analog for B2
    ANSELBbits.ANSB3 = 0;//Turn off analog for B3
    i2c_master_setup();
}

void setExpander(char pin,char level){
    i2c_master_start();
    i2c_master_send(0b1101011<<1|0);
    i2c_master_send(pin);
    i2c_master_send(level);
    i2c_master_stop();
}


unsigned char getExpander(){
    i2c_master_start();
    i2c_master_send(0b1101011<<1|0);
    i2c_master_send(0x0F);
    i2c_master_restart();
    i2c_master_send(0b1101011<<1|1);
    unsigned char r = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return r;
}

void I2CmultipleRead(unsigned char add, unsigned char reg, unsigned char* data, int length){
    i2c_master_start();
    i2c_master_send(add << 1 | 0);
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send(add << 1 | 1);
    int i = 0;
    while (i < (length - 1)) {
        data[i] = i2c_master_recv();
        i2c_master_ack(0);
        i++;
    }
    data[i] = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
}


//FROM HW6
void LCD_print_letter(short x,short y,char letter,char color1,char color2 ){
    letter=letter - 0x20;
    int i=0;
    for(i;i<5;i++){
        char col=ASCII[letter][i];
        int j=0;
        for(j;j<8;j++){
            if ((col >> j & 1) == 1){
                LCD_drawPixel(x+i,y+j,color1);
            }
            else{LCD_drawPixel(x+i,y+j,color2);
            }
        }
    }
}
void LCD_print_string(short x,short y,char* str,char color1,char color2){

    int j=0;
    while(str[j] && j <26){
        LCD_print_letter(x+j*5,y,str[j],color1,color2);
        j++;
    }

}
void LCD_progress_bar(short x,short y,short length,short height,char color){
    int i,j;
    for(i=0;i<length;i++){
        for(j=0;j<height;j++){
            LCD_drawPixel(x+i,y+j,color);
        }
    }
}

int main() {
    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    //led tests
    TRISAbits.TRISA4=0;
    LATAbits.LATA4=0;
    

    SPI1_init();
    LCD_init();
    __builtin_enable_interrupts();

  
    LCD_clearScreen(ILI9341_BLACK);

    
    
    initExpander();
    
    setExpander(0x10,0b10000010); //set up accelarometer
    setExpander(0x11,0b10001000);  //set all outputs as high
    setExpander(0x12,0b00000100);
    
   char message[26];
   unsigned char rawData[6];
   short data[3];

    
    int horiz;
    int vert;

   while(1){
       if(getExpander() != 0x69 ){
           sprintf(message,"Not reading who am i");
           LCD_print_string(10,200,message,ILI9341_RED,ILI9341_BLUE);
           while(1){
               ;
           }
       }
       if (_CP0_GET_COUNT() > 1200000){
           _CP0_SET_COUNT(0);
           I2CmultipleRead(0b1101011, 0x28, rawData, 6);
           int i=0;

           for(i;i<=2;i++){
               data[i] = (rawData[i*2 + 1] << 8) | rawData[i*2];      
       }
            
            sprintf(message, "x: %d    z: %d", data[0], data[1]);
            LCD_print_string(1, 1, message, ILI9341_WHITE, ILI9341_BLACK); 
            
           horiz = -1*data[0]*32/16000;
           vert = data[1]*32/16000;
           
            sprintf(message, "x_adj %d z_adj %d  ", horiz, vert);
            LCD_print_string(100, 220, message, ILI9341_RED, ILI9341_GREEN);
            
            
             if(horiz > 3) {
                LCD_progress_bar(14, 87, 47, 6, ILI9341_WHITE);
                LCD_progress_bar(67, 87, horiz-3,6, ILI9341_BLUE);
                LCD_progress_bar(67+horiz-3, 87, 50-horiz, 6, ILI9341_WHITE);
            }
            else if (horiz < -3) {
                LCD_progress_bar(14, 87, 50+horiz, 6, ILI9341_WHITE);
                LCD_progress_bar(61+horiz+3, 87, -1* horiz, 6, ILI9341_BLUE);
                LCD_progress_bar(68, 87, 47, 6, ILI9341_WHITE);
            }
            else {
                LCD_progress_bar(14, 87, 48, 6, ILI9341_WHITE);
                LCD_progress_bar(68, 87, 46, 6, ILI9341_WHITE);
            }
 
            if(vert > 3) {
                LCD_progress_bar(62, 40, 6, 47, ILI9341_WHITE);
                LCD_progress_bar(62, 93, 6, vert-3, ILI9341_BLUE);
                LCD_progress_bar(62, 93+vert-3, 6, 50-vert, ILI9341_WHITE);
            }
            else if (vert < -3) {
                LCD_progress_bar(62, 40, 6, 50+vert, ILI9341_WHITE);
                LCD_progress_bar(62, 87+vert+3, 6, -1*vert, ILI9341_BLUE);
                LCD_progress_bar(62, 93, 6, 47, ILI9341_WHITE);
            }
            else {
                LCD_progress_bar(62, 40, 6, 47, ILI9341_WHITE);
                LCD_progress_bar(62, 93, 6, 47, ILI9341_WHITE);
            }
       }
   }
         


    return (EXIT_SUCCESS);
}
