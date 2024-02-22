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
#include <stdbool.h>

#define A2D_VOLTAGE_REF_V  1.8 
#define A2D_MAX_READING    4095


static pthread_t printingThread;
static bool shutdown = false;


static void printSample(int dips){
    int sample = getHistorySize();
    int potentiometer[2] = {getVoltage0Reading(), getVoltage0Reading() / 40};
    double averageSample = getAverageVoltage();

    printf("Smpl/s = %4d\tPOT @ %4d => %4dHz\tavg = %.3fV\tdips = %4d", sample, potentiometer[0], potentiometer[1], averageSample, dips);
}

static void printTimingJitter(){
    Period_statistics_t pStats;
    Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &pStats);
    printf("\tSmpl ms[%6.3f, %6.3f] avg %.3f/%d\n", pStats.minPeriodInMs, pStats.maxPeriodInMs, pStats.avgPeriodInMs, pStats.numSamples);    
}

// Line 2: print 20 samples from the previous second
static void printRecentSamples(){
    int totalSamples = getHistorySize();

    double *history = getHistoryBuffer();
    printf("  ");
    
    if (totalSamples < 20) {
        for (int i = 0; i < totalSamples; i++) {
            printf("%d:%.3f  ", i, history[i]);
        }
    }
    else {
        int step = totalSamples / 20;  // Calculate the step size
        for (int i = 0; i < totalSamples; i += step) {
            printf("%d:%.3f  ", i, history[i]);
        }
    }
    printf("\n");

}

void *printing(void *args){
    (void)args;
    while(!shutdown){
        
        long long start = getTimeInMs();
        long long elapsed = 0;
        int dips = 0;

        while(elapsed < 1000){
            elapsed = getTimeInMs() - start;

            dips = analyzeLightDips();
            setDisplay(dips);

            pthread_mutex_lock(&data_mutex);

            printSample(dips);

            printTimingJitter();

            printRecentSamples();

            moveCurrentDataToHistory();
            setHistorySize(0);

            pthread_mutex_unlock(&data_mutex);

            // Period_statistics_t pStats;
            // Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &pStats);

            // Calculate the remaining time to wait before the next second
            long long remainingTime = 1000 - elapsed;

            // Check for positive remaining time before sleeping
            if (remainingTime > 0) {
                sleepForMs(remainingTime);
            }
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

