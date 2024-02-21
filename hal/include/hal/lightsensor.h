#ifndef _LIGHT_SENSOR_H_
#define _LIGHT_SENSOR_H_

#include <stdint.h>

void *updateVoltage1Reading(void *args);
int getVoltage1Reading();
double getRealWorldVoltage1();
void createLightSensorThread();
void joinLightSensorThread();

void shutdownLightSensor();

#endif