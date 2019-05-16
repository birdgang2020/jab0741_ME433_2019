/* 
 * File:   main.c
 * Author: jbera
 *
 * Created on May 16, 2019, 12:26 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
/*
 * 
 */

void setVoltage(char channel, int voltage){
    
}
void initSPI1(){
        RPB14Rbits.RPB14R = 0b; //Set B14 to SCK1
    RPB11Rbits.RPB11R = 0b0011; //Set B11 to SDO1
    RPB3bits.RPB3R=0b001l;//B3 to SS1
    SDI1bits.SDI1R = 0b0010; //Set A4 to SDI4
}


int main(int argc, char** argv) {

    
            
            
    return (EXIT_SUCCESS);
}

