#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#include <stdbool.h>
#include <pthread.h>


void setCurrentSize(int);
void setHistorySize(int);
void setHistoryDips(int);

pthread_mutex_t* get_mutex();
int getHistorySize();
int getCurrentSize();
int getHistoryDips();

long long getSamplesTaken();
double getAverageVoltage();
double *getHistoryBuffer();

void moveCurrentDataToHistory();
int analyzeLightDips();

void createSamplerThread();
void joinSamplerThread();
void shutdownSampler();

#endif
