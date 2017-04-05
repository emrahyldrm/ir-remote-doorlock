#ifndef SUB_UTILS_H
#define SUB_UTILS_H

#include <hidef.h>      /* common defines and macros */
#include <mc9s12dp256.h>     /* derivative information */


void ms_delay(int n);
void sn_delay(int n);
void us_delay(int n);

void itos(int n, char s[]);
void itosUn(unsigned int n, char s[]);
void reverse(char s[]);
int  mystrlen(char s[]);
void us50_delay(int n);


#endif