#include "sub_utils.h"

void us50_delay(int n) /* Delay n 50 microsecond intervals */
{
  unsigned int a,b;
  
  for(a=0; a<n; ++a)
    for(b=0; b<20; ++b);  
}


      
void ms_delay(int n) /* Delay n*1 millisecond intervals */
{
  volatile unsigned int a,b;
  
  for(a=0; a<n; ++a)
    for(b=0; b<4000; ++b);
}


void us_delay(int n) /* Delay n*1 millisecond intervals */
{
  volatile unsigned int a,b;
  
  for(a=0; a<n; ++a)
    for(b=0; b<4; ++b);
}



void sn_delay(int n) 
{
  volatile unsigned int a, b;
  
  for(a=0; a < n * 100 ; ++a)
    for(b=0; b < 40000; ++b);
}


void itos(int n, char s[]) 
{
  int i, sign;

  if ((sign = n) < 0) /* record sign */
    n = -n; /* make n positive */
  i = 0;
  do { /* generate digits in reverse order */
    s[i++] = n % 10 + '0'; /* get next digit */
  } while ((n /= 10) > 0); /* delete it */
 
  if (sign < 0)
    s[i++] = '-';
  
  s[i] = '\0';
  reverse(s);
}


void itosUn(unsigned int n, char s[]) 
{
  unsigned char i;

  i = 0;
  do { /* generate digits in reverse order */
    s[i++] = n % 10 + '0'; /* get next digit */
  } while ((n /= 10) > 0); /* delete it */

  s[i] = '\0';
  reverse(s);
}



/* reverse:  reverse string s in place */
void reverse(char s[]) 
{
  int i, j;
  char c;

  for (i = 0, j = mystrlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

int mystrlen(char s[]) 
{
  int len = 0;

  while ((*s) != '\0') 
  {
    ++s;
    ++len;
  }
  return len;
}
