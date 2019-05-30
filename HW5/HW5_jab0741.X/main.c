/* 
 * File:   main.c
 * Author: jbera
 *
 * Created on May 28, 2019, 1:46 PM
 */


#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>

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
    i2c_master_stop;
}
char getExpander(){
    i2c_master_start();
    i2c_master_send(0b0100000<<1|0);
    i2c_master_send(0x9);
    i2c_master_restart();
    i2c_master_send(0b0100000<<1|1);
    char r = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop;
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

    // do your TRIS and LAT commands here
    //Set TRIS register to declare I/O, 0 is output, 1 is input.
    TRISAbits.TRISA4 = 0;   //Declare RA4 (LED) as output
    TRISBbits.TRISB4 = 1;   //Declare RB4 (Push Button)
    //Note: Pins should already default to input, so above line may be unneccesary 
    
    initExpander();
    
    i2c_master_start();
    i2c_master_send(0b0100000 << 1 | 0);
    i2c_master_send(0x00);
    i2c_master_send(0xF0);
    i2c_master_stop();
    
    i2c_master_start();
    i2c_master_send(0b0100000 << 1 | 0);
    i2c_master_send(0x0A);
    i2c_master_send(0x0F);
    i2c_master_stop();
    
    while(1){
        if(getExpander() >>7 ==1){
        setExpander(0x,1);    
        
    }
        else{
            setExpander(0x,0)
        }
}

