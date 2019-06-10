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