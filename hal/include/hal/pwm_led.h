#ifndef _PWN_LED_H_
#define _PWN_LED_H_

#include <stdint.h>

void initializePWMLED();
void createPWMLEDThread();
void joinPWMLEDThread();
void shutdownPWMLED();

#endif