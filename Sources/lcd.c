#include "lcd.h"
#include "sub_utils.h"

void initLCD(void)
{
  DDRK = 0xFF; /* configure PORTK port for output */
  ms_delay(100); /* Wait for LCD to be ready */
  writeOne(0x28, 0); /* set 4-bit 1, 2-line display, 5x7 font */
  writeOne(0x0F, 0); /* turn on display, cursor, blinking */
  writeOne(0x06, 0); /* move cursor right */
  writeOne(0x01, 0); /* clear screen, move cursor to home */
  ms_delay(2); /* wait until "clear display" command complete */
}


void writeLine (char line, char *ptr)
{
  if(line == 0)
    writeOne(0x01, 0);
  else if(line == 1)
    writeOne(0xC0, 0);
  
  while (*ptr) 
  { /* While character to send */
    writeOne(*ptr, 1); /* Write 1 to LCD */
    us50_delay(1); /* Wait for 1 to be written */
    ptr++; /* Go to next character */
  } 
}

void writeOne(char c, char type) 
{
  char c_lo, c_hi;
  
  c_hi = (c & 0xF0) >> 2; /* Upper 4 bits of c */
  c_lo = (c & 0x0F) << 2; /* Lower 4 bits of c */
  
  PORTK &= (~0x01); /* select LCD command register */
  if (type == 1)
    PORTK = c_hi | 0x01; /* output upper 4 bits, E, RS high */
  else 
    PORTK = c_hi; /* output upper 4 bits, E, RS low */
    
  PORTK |= 0x02; /* pull E signal to high */
  __asm(nop); /* Lengthen E */
  __asm(nop);
  __asm(nop);
  PORTK &= (~0x02); /* pull E to low */
  
  /*lsb nibble*/
  
  PORTK &= (~0x01); /* select LCD command register */
  if (type == 1)
    PORTK = c_lo | 0x01; /* output lower 4 bits, E, RS high */
  else 
    PORTK = c_lo; /* output lower 4 bits, E, RS low */
  PORTK |= 0x02; /* pull E to high */
  __asm(nop); /* Lengthen E */
  __asm(nop);
  __asm(nop);
  PORTK &= (~0x02); /*pull E to low */
  
  ms_delay(1); /* Wait for command to execute */ 
}

