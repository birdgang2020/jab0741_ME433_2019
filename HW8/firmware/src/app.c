/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "ili9341.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
           __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

            // 0 data RAM access wait states
            BMXCONbits.BMXWSDRM = 0x0;

            // enable multi vector interrupts
            INTCONbits.MVEC = 0x1;

            // disable JTAG to get pins back
            DDPCONbits.JTAGEN = 0;
            ANSELB = 0;
            ANSELA = 0;
            // do your TRIS and LAT commands here
            TRISBbits.TRISB4=1;
            TRISAbits.TRISA4=0;
            LATAbits.LATA4=0;
            
            SPI1_init();
            LCD_init();
            initExpander();
            setExpander(0x10,0b10000010); //set up accelarometer
            setExpander(0x11,0b10001000);  //set all outputs as high
            setExpander(0x12,0b00000100);
            
            if (appInitialized)
            {
            
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {

    LCD_clearScreen(ILI9341_BLACK);

    
   char message[26];
   unsigned char rawData[6];
   short data[3];

    
    int horiz;
    int vert;
        _CP0_SET_COUNT(0);
   while(1){

           
       //HW1

            if(PORTBbits.RB4 !=0){
            if(_CP0_GET_COUNT()>2400000 && _CP0_GET_COUNT()<4800000 ){
                LATAbits.LATA4=0;
            }
            if(_CP0_GET_COUNT()>4800000){
                LATAbits.LATA4=1;
                           _CP0_SET_COUNT(0);
            }
            }



        
               if(getExpander() != 0x69 ){
           sprintf(message,"Not reading who am i");
           LCD_print_string(10,200,message,ILI9341_RED,ILI9341_BLUE);
           while(1){
               ;
           }
       }
        
       if (_CP0_GET_COUNT() > 1200000){
        sprintf(message,"LCD is On");            
        LCD_print_string(100,310,message,ILI9341_WHITE,ILI9341_PURPLE);

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
                    
                    
                    
                    
                    
            break;
        }
        
        
        

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
