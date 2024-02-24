#ifndef _SHUTDOWN_H_
#define _SHUTDOWN_H_

/* Module to initialize and cleanup the printing thread.
The printing thread handles printing sampler data from the previous second to the terminal every second.
It shares a lock data_mutex with sampler.c for thread-safe access to the history data.
*/

#include <stdbool.h>

void createThreads();
void joinThreads();
void waitShutdown();
void signalShutdown();

#endif