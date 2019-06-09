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
    i2c_master_send(0b1101011<<1|0);
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send(0b1101011<<1|1);
    int i=0;
    while(i< (length-1)){
        data[i]=i2c_master_recv();
        i2c_master_ack();
        i++;
    }
    data[i]=i2c_master_recv();
    i2c_master_ack();
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
    int i=0,j=0;
    for(i;i<length;i++){
        for(j;j<height;j++){
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
    LATAbits.LATA4=1;
  
    LCD_clearScreen(ILI9341_WHITE);

    
    
    initExpander();
    
    setExpander(0x10,0b10000010); //set up accelarometer
    setExpander(0x11,0b10001000);  //set all outputs as high
    setExpander(0x12,0b00000100);
    
   char message[26];
   int my_int=10;
   unsigned char rawData[14];
   short data[7];
   
   
   LCD_progress_bar(14, 87, 100, 6,ILI9341_PURPLE );
    LCD_progress_bar(22, 150, 100, 6,ILI9341_RED );
    LCD_progress_bar(30,200,100,6,ILI9341_BLUE);
    int horiz;
    int vert;

   while(1){
       if(getExpander() != 0x69 ){
           sprintf(message,"Not reading who am i")
           LCD_print_string(10,200,message,ILI9341_RED,ILI9341_BLUE);
           while(1){
               ;
           }
       }
       if (_CP0_GET_COUNT() > 1200000){
           _CP0_SET_COUNT(0);
           imuRead(0b1101011, 0x20, rawData, 14);
           
           
       }
       
       
       }
   }

    return (EXIT_SUCCESS);
}
