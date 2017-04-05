#include <hidef.h>            /* common defines and macros */
#include "derivative.h"       /* derivative-specific definitions */
#include "lcd.h"
#include "sub_utils.h"
#include "serial.h"
#include "keypad.h"
#include "string.h"



unsigned int number = 0;
unsigned int distance;
unsigned int count= 0;
unsigned int oldTCNT;
unsigned char flag = 0;
unsigned char intflag = 0;
unsigned int ovfIntCounter = 0;
unsigned char codi = 0;
unsigned char passcodi = 0;
unsigned char led_out = 255;
unsigned char a;
unsigned char tcrtsensor = 0;

char passcode[5];
char msg[17] = "";
char code[33];
char codes[21][33] = 
{
  "00000000111111110110100010010111",   //0
  "00000000111111110011000011001111",   //1
  "00000000111111110001100011100111",   //2
  "00000000111111110111101010000101",   //3
  "00000000111111110001000011101111",   //4
  "00000000111111110011100011000111",   //5    
  "00000000111111110101101010100101",   //6
  "00000000111111110101101010100101",   //7
  "00000000111111110101101010100101",   //8
  "00000000111111110101101010100101",   //9  
  "00000000111111111010001001011101",   //ch-
  "00000000111111110110001010011101",   //ch
  "00000000111111111110001000011101",   //ch+
  "00000000111111110010001011011101",   //<<
  "00000000111111110000001011111101",   //>>
  "00000000111111111100001000111101",   //>|
  "00000000111111111110000000011111",   //-
  "00000000111111111010100001010111",   //+
  "00000000111111111001000001101111",   //EQ
  "00000000011111111100110000110011",   //100+
  "00000000111111111011000001001111",   //200+

};

/*

char codes[18][33] = 
{
  "00000000111111111010001001011101",   //ch-
  "00000000111111110110001010011101",   //ch
  "00000000111111111110001000011101",   //ch+
  "00000000111111110010001011011101",   //<<
  "00000000111111110000001011111101",   //>>
  "00000000111111111100001000111101",   //>|
  "00000000111111111110000000011111",   //-
  "00000000111111111010100001010111",   //+
  "00000000111111111001000001101111",   //EQ
  "00000000111111110110100010010111",   //0
  "00000000011111111100110000110011",   //100+
  "00000000111111111011000001001111",   //200+
  "00000000111111110011000011001111",   //1
  "00000000111111110001100011100111",   //2
  "00000000111111110111101010000101",   //3
  "00000000111111110001000011101111",   //4
  "00000000111111110011100011000111",   //5    
  "00000000111111110101101010100101"    //6
};


char codes[18][33] = 
{
  "00000000111111111010001001011101",   
  "00000000111111110110001010011101",
  "00000000111111111110001000011101",
  "00000000111111110010001011011101",
  "00000000111111110000001011111101",
  "00000000111111111100001000111101",
  "00000000111111111110000000011111",
  "00000000111111111010100001010111",
  "00000000111111111001000001101111",
  "00000000111111110110100010010111",
  "00000000011111111100110000110011",
  "00000000111111111011000001001111",
  "00000000111111110011000011001111",
  "00000000111111110001100011100111",
  "00000000111111110111101010000101",
  "00000000111111110001000011101111",
  "00000000111111110011100011000111",
  "00000000111111110101101010100101"
};

*/
int findKey(char *opcode);
void playBuzzer(unsigned int msec_dur);
void light7Seg(char digit);



//buzzer pt4
//servo pwm pp7
//IR PT0
void main(void) 
{  
  DDRA = 0x00;  // may be keypad
  DDRB = 0xFF;  // leds all output
  DDRH = 0x00;  // dip switches all input
  DDRP |= DDRP_DDRP4_MASK;  // pwm out
  DDRP |= 0x0F;             // 7seg out
  DDRT &= ~DDRT_DDRT0_MASK;   //IR input PT0
  DDRT |= DDRT_DDRT4_MASK;    //make output PT4 for buzzer  


 
  TSCR1 |= 0x80;              //timer enable
  TSCR2 |= 0x80;              //no overflow interrupt
  TIOS  &= ~0x01;             //input capture from PORTT0
  TIE   |= 0x01;              //interrupt enable for PORTT0
  TCTL4 |= TCTL4_EDG0B_MASK;  //edge0A 0 edge0B 1 falling edge interrupt
  TCTL4 &= ~TCTL4_EDG0A_MASK; //edge0A 0 edge0B 1 falling edge interrupt
  TFLG1 |= TFLG1_C0F_MASK;    //
  TFLG2 |= TFLG2_TOF_MASK;    //
  
  EnableInterrupts;
  
  //TCRT5000 initalize
  ATD0CTL2 = 0x80;   //turn on adc
  us_delay(20);
  ATD0CTL3 = 0x08;
  ATD0CTL4 = 0xE3;
  
 /*  
  PWME = 0x00;
  PWMPRCLK|=0x01;       //ClockA=Fbus/2**2=12MHz/4=2MHz	
	PWMSCLA =200; 	      //ClockSA=3MHz/2x100=7500 Hz
	PWMCLK  =0b00010000; 	//ClockSA for chan 4
	PWMPOL  |= 0x10; 		  //high then low for polarity
	PWMCAE  =0x0; 		     //left aligned
	PWMCTL  =0x0;	         //8-bit chan, PWM during freeze and wait
	PWMPER4 =150; 	 //PWM_Freq=ClockSA/150=7500Hz/150=50Hz.
	PWMDTY4 =5; 	  //5% duty cycle           
	PWMCNT4 =0;		 //clear initial counter. This is optional	
	//PWME=0x10; 	   //Enable chan 4 PWM  
*/
 PWMPRCLK=0x04; //ClockA=Fbus/2**4=24MHz/16=1.5MHz	
	PWMSCLA=150; 	 //ClockSA=1.5MHz/2x150=5000 Hz
	PWMCLK=0b00010000; 	 //ClockSA for chan 4
	PWMPOL=0x10; 		     //high then low for polarity
	PWMCAE=0x0; 		     //left aligned
	PWMCTL=0x0;	         //8-bit chan, PWM during freeze and wait
	PWMPER4=100; 	 //PWM_Freq=ClockSA/100=6000Hz/100=60Hz. CHANGE THIS
	PWMDTY4=10; 	 //50% duty cycle           AND THIS TO SEE THE EFFECT ON MOTOR. TRY LESS THAN 10%
	PWMCNT4=10;		 //clear initial counter. This is optional
	//PWME=0x10; 	   //Enable chan 4 PWM   


  // DUTY CYCLE = 100%* [(PWMPERx - PWMDTYx)/PWMPERx]
  //serial monitor to putty
    
  count = 0;
  oldTCNT = 0;
  init_serial();  
  initLCD();        // Initialize LCD display


  strcpy(msg, "  Ciphered Door  ");
  writeLine(0, msg);
  strcpy(msg, "    Project    ");
  writeLine(1, msg);

  oldTCNT = TCNT;
  for(; ;) 
  {     
     if(PTH_PTH7 == 0b1) 
     {
        writeLine(0, "STAND BY..");
        passcodi = 0;
        flag = 1;
        TIE &= ~TIE_C0I_MASK;
     } 
     else if(flag == 1) 
     {
        writeLine(0, "RUNNING..");
        flag = 0;
        TIE |= TIE_C0I_MASK;
     }
     
     // busy waiting for stand by.
     while(PTH_PTH7 == 0b1);
     
     //if 4 digit entried
     if(passcodi == 4) 
     {
        TIE &= ~TIE_C0I_MASK;
        passcode[passcodi] = '\0';
        if(strcmp(passcode, "3535") == 0) 
        {
          strcpy(msg, "Opening !! ");
          writeLine(0, msg);
         
          PWMDTY4 = 5;
          ms_delay(10);
          PWME = 0x10;
          ms_delay(150);
         // PWME &= ~0x10;

          strcpy(msg, "Opened !! ");
          writeLine(0, msg);
          
          
          for(a = 11; a > 0; --a) 
          {
            
            light7Seg(a / 2);
            
            ovfIntCounter = 0;
            while(ovfIntCounter < 185); //500 ms delay       
            
            //read tcrt
            ATD0CTL5 = 0x82;
            while(!(ATD0STAT0 & ATD0STAT0_SCF_MASK));
            tcrtsensor = ATD0DR0L;
            
            if(tcrtsensor < 100) 
            {
              //ledout = 255;  
              a = 11;
            }                        
          }
          //PORTB = led_out = 0xFF;
          PORTB = 0;

          
          PWMDTY4 = 90;
          ms_delay(10);
          PWME = 0x10;
          ms_delay(150);
          
         // PWME &= ~0x10;          
          writeLine(0, "Closed !!");
        } 
        else 
        {
          strcpy(msg, "Wrong !! ");
          strcat(msg, passcode);
          writeLine(0, msg);
          playBuzzer(300);
        }

        TIE |= TIE_C0I_MASK;
        passcodi = 0;
        passcode[0] = '\0';
     }
  }
   

}
  
 

//pt0 interrupt
interrupt (((0x10000-Vtimch0)/2)-1) void TC0_ISR(void) 
{
  unsigned int key;  
  if(TC0 < oldTCNT)
    distance = 65535 - oldTCNT + TC0;
  else 
    distance = TC0 - oldTCNT;
  
  if(distance > 52000 && distance < 56000) 
  {
    code[codi] = '1';
    codi++;
  }
  else if(distance > 25000 && distance < 30000)
  {
    code[codi] = '0';
    codi++;
  }
  
  if(codi == 32) 
  {
    code[codi] = '\0';
    if( (key = findKey(code)) != 100) 
    {
       passcode[passcodi] = key + 48;
       passcodi++;
       
    }
    itos(key, msg);
    printSerial(code);
    printSerial(msg);
    codi = 0;
  }  
  
  oldTCNT = TC0;
  TFLG1 |= TFLG1_C0F_MASK;
}



//ovf interrupt
interrupt (((0x10000 - Vtimovf)/2)-1) void timovf(void) 
{
  ovfIntCounter++;
  TFLG2 |= TFLG2_TOF_MASK;
  
}
     
     
     
int findKey(char *opcode) 
{
  unsigned char i;
  for(i = 0; i < 21; ++i) 
  {
     if(strcmp(opcode, codes[i]) == 0) 
     {
        return i; 
     }
  }
  return 100;
}
             
             
void playBuzzer(unsigned int msec_dur) 
{
  unsigned char size = msec_dur / 10;
  for(a = 0; a < size; a++) 
  {
    PTT |= PTT_PTT4_MASK;
    ms_delay(5);
    PTT &= ~PTT_PTT4_MASK;
    ms_delay(5);
  }
}


void light7Seg(char digit) 
{    
  PTP = 0x07;
  
  if(digit == 0) 
  {
    PORTB = 0x3F;     
    ms_delay(50);
  } 
  else if(digit == 1) 
  {
    PORTB = 0x06;     
    ms_delay(50);
  }
  else if(digit == 2) 
  {
    PORTB = 0x5B;     
    ms_delay(50);
  }
  else if(digit == 3) 
  {
    PORTB = 0x4F;     
    ms_delay(50);
  }
  else if(digit == 4) 
  {
    PORTB = 0x66;     
    ms_delay(50);
  }
  else if(digit == 5) 
  {
    PORTB = 0x6D;     
    ms_delay(50);
  }
  else if(digit == 6) 
  {
    PORTB = 0x7D;     
    ms_delay(50);
  }
  else if(digit == 7) 
  {
    PORTB = 0x07;     
    ms_delay(50);
  }
  else if(digit == 8) 
  {
    PORTB = 0x7F;     
    ms_delay(50);
  }
  else if(digit == 9) 
  {
    PORTB = 0x6F;     
    ms_delay(50);
  }
  
  
}







