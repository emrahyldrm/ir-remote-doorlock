#ifndef KEYPAD_H
#define KEYPAD_H

#include <hidef.h>            /* common defines and macros */
#include "derivative.h" 
#include "sub_utils.h" 

#define ROW 4
#define COL 4

// Gets pressed key number from keypad
// Waits for any key to pressed otherwise won't return.
int get_key();


#endif