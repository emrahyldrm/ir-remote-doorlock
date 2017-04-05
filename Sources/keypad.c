#include "keypad.h"


// Gets pressed key number from keypad
// Waits for any key to pressed otherwise won't return.
int get_key() 
{
    int row,
        column;
                
    const char row_mask[ROW] = { 0xFE,0xFD,0xFB,0xF7 };
    const char col_mask[COL] = { 0x10,0x20,0x40,0x80 };        
    const unsigned int keys[ROW][COL] = { 1,2,3,10,
                                          4,5,6,11,
                                          7,8,9,12,
                                          14,0,15,13 };
                
    for(row=0 ; row < ROW ; ++row){
        
        PORTA = 0xFF;// Reset PortA
        for(column=0; column < COL ;++column){
                  
            PORTA = PORTA & row_mask[row]; // Set specific row to 0
                                      
            if( (PORTA & col_mask[column]) == 0 ){
            // Check if any key is pressed
                
                  ms_delay(50);                      
                  // Wait 50ms and check again for make sure key is pressed.
                  if( (PORTA & col_mask[column]) == 0 ){
                        return keys[row][column]; 
                  }                   
                 
            }          
            
        }
    
    }    
    
    return -1;
    
}