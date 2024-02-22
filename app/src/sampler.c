#include "sampler.h"
#include "hal/display.h"
#include "hal/lightsensor.h"
#include "hal/pwm_led.h"
#include "hal/potentiometer.h"
#include "hal/general_command.h"
#include "period_timer.h"

#include "mutex.h"

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 2000
#define DATA_WEIGHT 0.001

static pthread_t sampleThread;

static bool shutdown = false;

static int historySize = 0;
static long long samplesTaken = 0;
static double averageVoltage = 0.0;
static double currentBuffer[BUFFER_SIZE];
static double historyBuffer[BUFFER_SIZE];


void setHistorySize(int size){
    historySize = size;
}

int getHistorySize(){
    return historySize;
}

long long getSamplesTaken(){
    return samplesTaken;
}

double getAverageVoltage(){
    return averageVoltage;
}


double *getHistoryBuffer(){
    return historyBuffer;

    // pthread_mutex_lock(&data_mutex);

    // // Copy samples to a new array
    // double* history_copy = (double*)malloc(historySize * sizeof(double));
    // for (int i = 0; i < historySize; ++i) {
    //     history_copy[i] = historyBuffer[i];
    // }

    // pthread_mutex_unlock(&data_mutex);

    // return history_copy;
}

void moveCurrentDataToHistory() {

    //pthread_mutex_lock(&data_mutex);
    


    //free(historyBuffer); // Free previous history

    // Move current samples to history
     for (int i = 0; i < historySize; ++i) {
        historyBuffer[i] = currentBuffer[i];
    }

    // Reset current samples
    //currentBuffer = NULL;
    //currentSize = 0;

    //pthread_mutex_unlock(&data_mutex);
}

int analyzeLightDips() {

    pthread_mutex_lock(&data_mutex);

    double* historySamples = historyBuffer;
    int size = historySize;
    double currentAverage = averageVoltage;

    pthread_mutex_unlock(&data_mutex);

    int dipCount = 0;

    // Define parameters for detecting dips
    double dipThreshold = 0.1;
    double hysteresis = 0.03;
    double dipTriggerThreshold = currentAverage - dipThreshold;
    double dipRecoveryThreshold = currentAverage - hysteresis;

    bool dipInProgress = false;

    // Analyze history for dips
    for (int i = 0; i < size; ++i) {

        if (historySamples[i] <= dipTriggerThreshold) {

            if (!dipInProgress) {
                // Start a new dip
                dipInProgress = true;
                dipCount++;
            }
        } else if (historySamples[i] >= dipRecoveryThreshold) {
            // Reset dip status when light level recovers
            dipInProgress = false;
        }
    }

    return dipCount;
}

static void *sampleLight(void *args){
    (void)args;
    while(!shutdown){
        
        pthread_mutex_lock(&data_mutex);
        
        Period_markEvent(PERIOD_EVENT_SAMPLE_LIGHT);

        double light = getRealWorldVoltage1();
        currentBuffer[historySize] = light;
        historySize++;
        samplesTaken++;

        // not sure, probably unnecessary
        if(historySize == BUFFER_SIZE) {
            historySize = 0;
        }
        
        if(historySize == 0)
            averageVoltage = light;
        else 
            averageVoltage = light;

        pthread_mutex_unlock(&data_mutex);

        sleepForMs(1);
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

