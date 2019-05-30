/* 
 * File:   main.c
 * Author: jbera
 *
 * Created on May 28, 2019, 1:46 PM
 */


#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
#include "i2c_master_noint.h"

void initExpander(){
    ANSELBbits.ANSB2 = 0;//Turn off analog for B2
    ANSELBbits.ANSB3 = 0;//Turn off analog for B3
    i2c_master_setup();
}

void setExpander(char pin,char level){
    i2c_master_start();
    i2c_master_send(0b0100000<<1|0);
    i2c_master_send(pin);
    i2c_master_send(level);
    i2c_master_stop();
}

unsigned char getExpander(){
    i2c_master_start();
    i2c_master_send(0b0100000<<1|0);
    i2c_master_send(0x9);
    i2c_master_restart();
    i2c_master_send(0b0100000<<1|1);
    unsigned char r = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return r;
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

    TRISBbits.TRISB15=0;
    LATBbits.LATB15=1;
    
    initExpander();
    
    __builtin_enable_interrupts();
    
    //Write to IODIR register to initialize GPIO7 as output
    i2c_master_start();
    i2c_master_send(0b0100000 << 1 | 0);
    i2c_master_send(0x00);
    i2c_master_send(0x80);
    i2c_master_stop();
    
    //Write to OLAT register to initialize GPIO0 as input
    i2c_master_start();
    i2c_master_send(0b0100000 << 1 | 0);
    i2c_master_send(0x0A);
    i2c_master_send(0x01);
    i2c_master_stop();
    
    _CP0_SET_COUNT(0);
    
    while(1){
        if(getExpander() >>7 ==1){
        setExpander(0xA,1);    
        
    }
        else{
            setExpander(0xA,0);
        }
        if(_CP0_GET_COUNT()>1200000){
            LATBbits.LATB15=!LATBbits.LATB15;
            _CP0_SET_COUNT(0);
        }
}
}

