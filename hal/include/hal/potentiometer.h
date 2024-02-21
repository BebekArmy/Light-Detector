#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_

#include <stdint.h>

void *updateVoltage0Reading(void *args);
int getVoltage0Reading();
double getRealWorldVoltage0();
void createPotentiometerThread();
void joinPotentiometerThread();

void shutdownPotentiometer();

#endif