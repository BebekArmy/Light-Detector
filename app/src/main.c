// Main program to build the application

#include "hal/general_command.h"
#include "hal/display.h"
#include "hal/potentiometer.h"
#include "hal/lightsensor.h"
#include "hal/pwm_led.h"
#include "udp.h"
#include "period_timer.h"
#include "sampler.h"
#include "print_result.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define MSG_MAX_LEN 1024
#define PORT        12345

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

int main()
{
    bool isRunning = true;

    createThreads();

    while(isRunning){
        isRunning = getProgramRunning();
    }

    joinThreads();
    

    return 0;
}