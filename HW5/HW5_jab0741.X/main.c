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
    
}
int main() {

}

