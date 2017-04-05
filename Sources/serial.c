#include "serial.h"

void init_serial()
{
	//serial monitor to putty
	SCI0BDH = 0x00;
	SCI0BDL = 0x9C; //baud rate
	SCI0CR1 = 0x00;
	SCI0CR2 = 0x0C;
}

void printSerial(char *msg) 
{
	unsigned char i = 0;
	while(msg[i] != '\0')
	{
		while(! (SCI0SR1 & SCI0SR1_TDRE_MASK));
		SCI0DRL = msg[i];
		i++;
	}
	while(! (SCI0SR1 & SCI0SR1_TDRE_MASK));
	SCI0DRL = ' ';
	while(! (SCI0SR1 & SCI0SR1_TDRE_MASK));
	SCI0DRL = '\r';
	while(! (SCI0SR1 & SCI0SR1_TDRE_MASK));
	SCI0DRL = '\n';  
}

