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


void testDisplay()
{
    initializeDisplay();

    createDisplayThread();
    
    for(int i = 0; i < 13; i++)
    {
        setDisplay(i);
        sleepForMs(500);
    }

    
}

void testPotentiometer()
{
    createPotentiometerThread();

    int n = 0;
    while(n<5){
        sleepForMs(1000);
        printf("Voltage Reading: %d\n", getVoltage0Reading());
        printf("Real World Voltage: %f\n", getRealWorldVoltage0());
        
        n++;
    }

    
}


void testLightSensor()
{
    createLightSensorThread();

    int n = 0;
    while(n<5){
        sleepForMs(1000);
        printf("Voltage Reading: %d\n", getVoltage1Reading());
        printf("Real World Voltage: %f\n", getRealWorldVoltage1());
        
        n++;
    }

    
}

void testPWMLED()
{
    initializePWMLED();

    createPotentiometerThread();
    createPWMLEDThread();

    sleepForMs(10000);


}


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
    // testDisplay();

    // testPotentiometer();

    // testLightSensor();

    // testPWMLED();



    // shutdownDisplay();
    // joinDisplayThread();

    // shutdownLightSensor();
    // joinLightSensorThread();

    // shutdownPWMLED();
    // joinPWMLEDThread();

    // shutdownPotentiometer();
    // joinPotentiometerThread();

    // createUDPThread();
    // joinUDPThread();

    bool isRunning = true;

    createThreads();

    while(isRunning){
        isRunning = getProgramRunning();
    }

    joinThreads();
    



    

    return 0;
    
}