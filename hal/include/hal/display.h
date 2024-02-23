#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>


void runCommandDisplay(char* command);
void initializeDisplay();
void setDisplay(int value);
void *displayDigits(void *args);
void createDisplayThread();
void joinDisplayThread();
void shutdownDisplay();


#endif