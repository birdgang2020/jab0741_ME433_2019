/* 
 * File:   main.c
 * Author: jbera
 *
 * Created on June 14, 2019, 8:34 PM
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
    void __ISR(_TIMER_3_VECTOR, IPL5SOFT) TIMER3(void) {
        if(OC1RS>0){
    OC1RS -=24;
        }
        
    IFS0bits.T3IF = 0;
  
}
    
    char message [40];
    char array1[1]={"a"};
    char array2[1]={"b"};
    char array3[1]={"c"};
    LCD_display(const char* array,int x,int y){
        int i=0;
        for(i;i<sizeof(array);i++){
            char c=((array[i]>>5) | 0b00000000);
            sprintf(message,"%c",c);
            LCD_print_string(x,y,message,ILI9341_WHITE,ILI9341_BLACK);
        }
    }
    
    void Timer2_INIT(){
    T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)

    PR2 = 2399; // PR = PBCLK / N / desiredF - 1

    TMR2 = 0; // initial TMR2 count is 0

    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults

    OC1RS = 1000; // duty cycle

    OC1R = 0; // initialize before turning OC1 on; afterward it is read-only

    T2CONbits.ON = 1; // turn on Timer2

    OC1CONbits.ON = 1; // turn on OC1
    
    RPB4R=0b0101;
    }
    
    void Timer3_INIT(){
    T3CONbits.TCKPS = 0b111; // Timer2 prescaler N=1 (1:1)

    PR3 =1875 ; // PR = PBCLK / N / desiredF - 1

    TMR3 = 0; // initial TMR2 count is 0

    T3CONbits.ON = 1; // turn on Timer2
     
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
    Timer2_INIT();
    Timer3_INIT();
    
//Timer4 and Timer5 ISRs
            // INT step 4: priority
  IPC3bits.T3IP=5;        //             subpriority
  IPC3bits.T3IS=0;           // INT step 5: clear interrupt flag
  IFS0bits.T3IF=0;            // INT step 6: enable interrupt
  IEC0bits.T3IE =1;



   __builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU
   LCD_clearScreen(ILI9341_BLACK);
 
  LCD_display(array1,100,100);
  LCD_display(array2,150,100);
  LCD_display(array3,200,200);
   while(1){
   if (_CP0_GET_COUNT()>48000000){
       OC1RS=2399;
       LATAINV=0b10000;
       _CP0_SET_COUNT(0);
   }
}
}




