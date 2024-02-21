#ifndef _GENERAL_COMMAND_H_
#define _GENERAL_COMMAND_H_

#include <stdint.h>

long long getTimeInMs(void);
void sleepForMs(long long delayInMs);
void runCommand(char* command);
void writeToFile(const char *file, const char *value);


#endif