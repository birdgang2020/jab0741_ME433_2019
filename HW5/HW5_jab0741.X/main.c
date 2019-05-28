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
    
}
void setExpander(char pin,char level){
    
}
char getExpander(){
    
}
int main() {

}

