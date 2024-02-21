#include "hal/pwm_led.h"
#include "hal/general_command.h"
#include "hal/potentiometer.h"
#include "hal/lightsensor.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#define PWM_LED_PATH "/dev/bone/pwm/0/b"

static bool shutdown = false;

static pthread_t pwmLEDThread;

static int previousHz = 0;

void initializePWMLED(){
    runCommand("config-pin P9_21 pwm");
    writeToFile(PWM_LED_PATH "/enable", "0");
    writeToFile(PWM_LED_PATH "/enable", "1");

}

//set pwm led according to the raw value of potentiometer. raw value is between 0 and 4095. set the led to be raw value / 40 Hz. make the duty cycle to be 50%.

void *setPWMLED(void *args){
    (void)args;
    
    while(!shutdown){
        int rawValue = getVoltage0Reading();
        int hz = rawValue/ 40;

        if(previousHz == hz){
            //printf("light sensor reading: %d\n", getVoltage1Reading());
            sleepForMs(100);
            continue;
        }

        else{
            if(hz < 1){
                hz = 1;
            }
            previousHz = hz;
            printf("raw value: %d\n", rawValue);
            printf("Setting PWM LED to %d Hz\n", hz);


            int period = 1000000000 / hz;
            int duty_cycle = period / 2;



            char periodStr[20];
            char dutyStr[20];

            sprintf(periodStr, "%d", period);
            sprintf(dutyStr, "%d", duty_cycle);

            writeToFile(PWM_LED_PATH "/duty_cycle", "0");
            writeToFile(PWM_LED_PATH "/period", periodStr);
            writeToFile(PWM_LED_PATH "/duty_cycle", dutyStr);
            sleepForMs(100);
        }

        //printf("light sensor reading: %d\n", getVoltage1Reading());
    }
    writeToFile(PWM_LED_PATH "/enable", "0");
    return NULL;
}

void createPWMLEDThread(){
    printf("Creating PWM LED Thread\n");
    pthread_create(&pwmLEDThread, NULL, setPWMLED, NULL);
}

void joinPWMLEDThread(){
    printf("Joining PWM LED Thread\n");
    pthread_join(pwmLEDThread, NULL);
}

void shutdownPWMLED(){
    printf("Shutting down PWM LED\n");
    shutdown = true;
}

