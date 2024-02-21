#include "sampler.h"
#include "hal/display.h"
#include "hal/lightsensor.h"
#include "hal/pwm_led.h"
#include "hal/potentiometer.h"
#include "hal/general_command.h"
#include "period_timer.h"

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 2000
#define DATA_WEIGHT 0.001

static pthread_t sampleThread;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static bool shutdown = false;

static int historySize = 0;
static int samplesTaken = 0;
static int averageVoltage = 0;
static int averageReading = 0;
static int currentBuffer[BUFFER_SIZE];
static int historyBuffer[BUFFER_SIZE];


int getHistorySize(){
    return historySize;
}

int getSamplesTaken(){
    return samplesTaken;
}

int getAverageVoltage(){
    return averageVoltage;
}


int *getHistoryBuffer(){
    return historyBuffer;
}

static void *sampleLight(void *args){
    (void)args;
    while(!shutdown){
        
        pthread_mutex_lock(&mutex);

        int light = getVoltage1Reading();
        currentBuffer[historySize] = light;
        historySize++;
        samplesTaken++;

        if(historySize == 0)
            averageReading = light;
        else 
            averageReading = light * DATA_WEIGHT + averageReading * (1 - DATA_WEIGHT);

        Period_markEvent(PERIOD_EVENT_SAMPLE_LIGHT);
            
        sleepForMs(1);

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void createSamplerThread(){
    pthread_create(&sampleThread, NULL, &sampleLight, NULL);
}

void joinSamplerThread(){
    pthread_join(sampleThread, NULL);
}

void shutdownSampler(){
    shutdown = true;
}

