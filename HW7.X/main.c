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

   char message[26];
   int my_int=10;


   
   int count=0;
   u_int last_tick=0;
   u_int current_tick=0;

   while(1){
       sprintf(message, "Hello World %d",my_int);
       LCD_print_string(28, 50, message, ILI9341_BLACK, ILI9341_WHITE);
          //White bar won't go away on my LCD so displaying "Hello World" slightly lower
     if(count<86){
         LCD_progress_bar(28+count,70,my_int,8,ILI9341_PURPLE);
     }
     current_tick=_CP0_GET_COUNT();
     if(current_tick % 10 ==9){
        sprintf(message, "fps:%g", 24000000/((float)current_tick - last_tick));
        LCD_print_string(40,150, message, ILI9341_BLACK, ILI9341_WHITE );
     }
        last_tick = current_tick;
        
        if (_CP0_GET_COUNT() > 480000){
            if(my_int<100){
                my_int+=1;
                
            }
            _CP0_SET_COUNT(0);
            LATAINV = 0b10000;
            count++;
            last_tick = 0;
        }
   }

    return (EXIT_SUCCESS);
}
