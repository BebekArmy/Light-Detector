#include "hal/general_command.h"
#include "hal/display.h"
#include "hal/potentiometer.h"
#include "hal/lightsensor.h"
#include "hal/pwm_led.h"
#include "udp.h"
#include "period_timer.h"
#include "sampler.h"
#include "print_result.h"

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

pthread_mutex_t shutdowMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t shutdownCond = PTHREAD_COND_INITIALIZER;

void createThreads()
{
    Period_init();
    
    initializeDisplay();
    createDisplayThread();

    createPotentiometerThread();

    initializePWMLED();
    createPWMLEDThread();

    createLightSensorThread();

    createUDPThread();

    createSamplerThread();

    createPrintingThread();
}

void joinThreads()
{
    shutdownSampler();
    shutdownPrintingThread();
    shutdownDisplay();
    shutdownPWMLED();
    shutdownPotentiometer();
    shutdownLightSensor();


    joinSamplerThread();
    joinPrintingThread();
    joinUDPThread();
    joinDisplayThread();
    joinPotentiometerThread();
    joinPWMLEDThread();
    joinLightSensorThread();

    Period_cleanup();
}

void waitShutdown()
{
    pthread_mutex_lock(&shutdowMutex);
    pthread_cond_wait(&shutdownCond, &shutdowMutex);
    pthread_mutex_unlock(&shutdowMutex);
}

void signalShutdown()
{
    pthread_mutex_lock(&shutdowMutex);
    pthread_cond_signal(&shutdownCond);
    pthread_mutex_unlock(&shutdowMutex);
}