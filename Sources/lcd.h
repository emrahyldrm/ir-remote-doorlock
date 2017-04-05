#ifndef LCD_H
#define LCD_H

#include <hidef.h>      /* common defines and macros */
#include <mc9s12dp256.h>     /* derivative information */


void initLCD(void); /* Initialize LCD display */
void writeOne(char c, char type); /* Write command or data to LCD controller */
void writeLine (char line, char *ptr); /* Write a string to the LCD display */


#endif