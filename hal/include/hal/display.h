#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>

int initI2cBus(char* bus, int address);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);
void writeToFile(const char *file, const char *value);
void runCommandDisplay(char* command);
void initializeDisplay();
void setDisplay(int left, int right);
void *displayDigits(void *args);
void createDisplayThread();
void joinDisplayThread();
void shutdownDisplay();


#endif