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
int count=0;
void XPT2046_read(int *z, unsigned short *x, unsigned short *y){
    
    CST = 1; // CS starts high
    unsigned short z1, z2;
    
    unsigned char r1, r2;
    unsigned short t1, t2;
  
    CST = 0;
    spi_io(0b10110001); // Z1
    r1 = spi_io(0x00);
    r2 = spi_io(0x00);
    CST = 1;
    z1 = ((r1<<8)|r2)>>3;
    
    CST = 0;
    spi_io(0b11000001); // Z2
    r1 = spi_io(0x00);
    r2 = spi_io(0x00);
    CST = 1;
    z2 = ((r1<<8)|r2)>>3;
    
    CST = 0;
    spi_io(0b10010001); // Y
    r1 = spi_io(0x00);
    r2 = spi_io(0x00);
    CST = 1;
    t1 = ((r1<<8)|r2)>>3;
    
    CST = 0;
    spi_io(0b11010001); // X
    r1 = spi_io(0x00);
    r2 = spi_io(0x00);
    CST = 1;
    t2 = ((r1<<8)|r2)>>3;
   
    
    *z = z1 - z2 + 4095;
    *y = t1;
    *x=t2;
}

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
            TRISAbits.TRISA4=0;
            TRISBbits.TRISB5 = 0; // CST is B5
            
            SPI1_init();

            LCD_init();
            LCD_clearScreen(ILI9341_BLACK);
            if (appInitialized)
            {
            
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }
        
        case APP_STATE_SERVICE_TASKS:
        {
            
            unsigned short x, y; int z;
            XPT2046_read(&z,&x,&y);

            char str[30];

            while(_CP0_GET_COUNT()<12000000){
                ;
            }
            _CP0_SET_COUNT(0);
            int x_adj=(x-400)/14;
            int y_adj;
            int z_adj;
            if(y==0){
                
            }
            
            else{
                y_adj=340-(y-250)/10.5;
            }
            if(z>400){
                z_adj=1;
             
            }
            else{
                z_adj=0;
            }
            sprintf(str,"x adj:%d y adj:%d",x_adj,y_adj);
            LCD_print_string(0,40,str,ILI9341_WHITE,ILI9341_BLACK);
            _CP0_SET_COUNT(0);
        
        
        LCD_progress_bar(150,150,10,10,ILI9341_WHITE);
        LCD_progress_bar(200,200,10,10,ILI9341_GREEN);
        if((x_adj>140&& x_adj<170)&&(y_adj>140 && y_adj<170)){
            count+=1;
        }
        if((x_adj>180 && x_adj<220)&&(y_adj>180 && y_adj<220)){
            count-=1;
        }
        sprintf(str,"COUNT: %d",count);
        LCD_print_string(0,200,str,ILI9341_WHITE,ILI9341_BLACK);
        /* TODO: implement your application state machine.*/
        }

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
