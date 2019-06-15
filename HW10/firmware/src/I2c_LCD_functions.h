/* 
 * File:   I2c_LCD_functions.h
 * Author: jbera
 *
 * Created on June 13, 2019, 7:56 PM
 */

#ifndef I2C_LCD_FUNCTIONS_H
#define	I2C_LCD_FUNCTIONS_H

void initExpander();
void setExpander(char pin,char level);
unsigned char getExpander();
void I2CmultipleRead(unsigned char add, unsigned char reg, unsigned char* data, int length);
void LCD_print_letter(short x,short y,char letter,char color1,char color2 );
void LCD_print_string(short x,short y,char* str,char color1,char color2);
void LCD_progress_bar(short x,short y,short length,short height,char color);

#endif	/* I2C_LCD_FUNCTIONS_H */

