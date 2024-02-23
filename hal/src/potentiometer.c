#include "hal/potentiometer.h"
#include "hal/general_command.h"
#include <stdlib.h> 
#include <stdbool.h> 
#include <stdio.h> 
#include <pthread.h>

#define A2D_FILE_VOLTAGE0  "/sys/bus/iio/devices/iio:device0/in_voltage0_raw" 
#define A2D_VOLTAGE_REF_V  1.8 
#define A2D_MAX_READING    4095

static bool shutdown = false;
static int a2dReading = 0;
static pthread_t potentiometerThread;


void *updateVoltage0Reading(void *args) { 
    // Open file 
    (void)args;
    while(!shutdown){
        FILE *f = fopen(A2D_FILE_VOLTAGE0, "r"); 
        if (!f) { 
            printf("ERROR: Unable to open voltage input file. Cape loaded?\n"); 
            printf("       Check /boot/uEnv.txt for correct options.\n"); 
            exit(-1); 
        } 
        // Get reading 
        
        int itemsRead = fscanf(f, "%d", &a2dReading); 
        //print itemsRead
        if (itemsRead < 0) { 
                printf("ERROR: Unable to read values from voltage input file.\n"); 
                exit(-1); 
        } 
        fclose(f); 
    }
    
    return NULL;
}

int getVoltage0Reading() { 
    return a2dReading; 
}

double getRealWorldVoltage0(){
    return (a2dReading * A2D_VOLTAGE_REF_V) / A2D_MAX_READING;
}

void createPotentiometerThread() {
    pthread_create(&potentiometerThread, NULL, updateVoltage0Reading, NULL);
}

void joinPotentiometerThread() {
    pthread_join(potentiometerThread, NULL);
}

void shutdownPotentiometer() { 
    shutdown = true; 
}