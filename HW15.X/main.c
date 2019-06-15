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
    void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Current_control_ISR(void) {
        OC1RS=
  
    IFS0bits.T2IF = 0;
  
}
    void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Current_control_ISR(void) {
        
        LATAINV=0b10000;
    IFS0bits.T3IF = 0;
  
}
    
    
    void Timer2_INIT(){
    T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)

    PR2 = 2399; // PR = PBCLK / N / desiredF - 1

    TMR2 = 0; // initial TMR2 count is 0

    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults

    OC1RS = 0; // duty cycle

    OC1R = 0; // initialize before turning OC1 on; afterward it is read-only

    T2CONbits.ON = 1; // turn on Timer2

    OC1CONbits.ON = 1; // turn on OC1
    
    RPB4R=0b0101;
    }
    
    void Timer3_INIT(){
    T3CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)

    PR3 = 2399; // PR = PBCLK / N / desiredF - 1

    TMR3 = 0; // initial TMR2 count is 0

    OC2CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults

    OC2RS = 0; // duty cycle

    OC2R = 0; // initialize before turning OC1 on; afterward it is read-only

    T3CONbits.ON = 1; // turn on Timer2

    OC2CONbits.ON = 1; // turn on OC1
    
           
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
//Timer4 and Timer5 ISRs
  IPC2bits.T2IP = 3;              // INT step 4: priority
  IPC3bits.T3IP=5;
  IPC2bits.T2IS = 0;              //             subpriority
  IPC3bits.T3IS=0;
  IFS0bits.T2IF = 0;              // INT step 5: clear interrupt flag
  IFS0bits.T3IF=0;
  IEC0bits.T2IE = 1;              // INT step 6: enable interrupt
  IEC0bits.T3IE =1;



   __builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU
    

    LCD_clearScreen(ILI9341_WHITE);

   char message[26];
   int my_int=10;


   
   int count=0;
   u_int last_tick=0;
   u_int current_tick=0;

   while(1){


    return (EXIT_SUCCESS);
}
}



