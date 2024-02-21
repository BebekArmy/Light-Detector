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

#define A2D_VOLTAGE_REF_V  1.8 
#define A2D_MAX_READING    4095


static pthread_t printingThread;
static bool shutdown = false;


static void printSample(){
    int sample = getHistorySize();
    int potentiometer[2] = {getVoltage0Reading(), getRealWorldVoltage0()};
    double averageSample = (getAverageVoltage() * A2D_VOLTAGE_REF_V) / A2D_MAX_READING;
    //int *history = getHistoryBuffer();


    printf("Smpl/s = %d POT @ %d = %dHz avg = %.3fV dips = %d \n", sample, potentiometer[0], potentiometer[1], averageSample, 0);
    //printf("Smpl ms [ %.3f, %.3f, %.3f] avg %.3f/%d\n", minSample, maxSample, averageSample, averageSample, samples);
}

static void timingJitter(){
    Period_statistics_t pStats;
    Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &pStats);
    
}

void *printing(void *args){
    (void)args;
    while(!shutdown){
        
        long long start = getTimeInMs();
        long long elapsed = 0;

        while(elapsed < 1000){
            elapsed = getTimeInMs() - start;
            printSample();
        
            Period_statistics_t pStats;
            Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &pStats);
        }


        

        
    }
    return NULL;
}

void createPrintingThread(){
    pthread_create(&printingThread, NULL, printing, NULL);
}

void joinPrintingThread(){
    pthread_join(printingThread, NULL);
}

void shutdownPrintingThread(){
    shutdown = true;
}

