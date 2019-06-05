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
void LCD_print_letter(short x,short y,char letter,char color1,char color2 ){
    letter=letter=0x20;
    int i=0;
    for(i;i<5;i++){
        char col=ASCII[letter][i];
        int j=0;
        for(j;j<8;j++){
            char current_bit=(col>>j & 1);
            if (current_bit==1){
                LCD_drawPixel(x+i,y+j,color1);
            }
            else{LCD_drawPixel(x+i,y+j,color2);
            }
        }
    }
}
void LCD_print_string(short x,short y,char* str,char color1,char color2){
    int j=0;
    while((str[j] &&j )<26){
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
    
        while(1){
            if (_CP0_GET_COUNT() >24000000){
        _CP0_SET_COUNT(0);
        LATAbits.LATA4= !LATAbits.LATA4;
        }
    }

    LCD_init();
    

    
    LCD_clearScreen(0x0000);
    LCD_drawPixel(100,100,ILI9341_BLACK);
    char message[26];
    sprintf(message, "Hello World");
    LCD_print_string(35, 65, message, 0xFFFF, 0x0000);
    
    LCD_progress_bar(20, 100, 88, 10, ILI9341_WHITE);
    LCD_progress_bar(21, 101, 86, 8, ILI9341_BLACK);
    
    __builtin_enable_interrupts();

    //Set PIC32 internal clock to 0
    _CP0_SET_COUNT(0);
    /*
    int count=0;
    u_int last_tick=0;
    u_int current_tick=0;
    while(1){
        
        if(count<86){
            LCD_progress_bar(18+count,100,1,8,ILI9341_RED);
        }
        current_tick=_CP0_GET_COUNT();
        sprintf(message, "fps:%g", 24000000/((float)current_tick - last_tick));
        LCD_print_string(1,1,message,ILI9341_WHITE,ILI9341_BLACK);
        last_tick=current_tick;
         
            if (_CP0_GET_COUNT() >24000000){
            _CP0_SET_COUNT(0);
            LATAbits.LATA4= !LATAbits.LATA4;
            count++;
            last_tick = 0;
        }
    }
     * */
     
    
    
    return (EXIT_SUCCESS);
}

