#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <cstdio>
#include <stdio.h>

// Assuming led1 and led2 are defined elsewhere in your code
// extern int led1;
// extern int led2;

// Function prototype for the monitor function
extern void monitor(void);

// Task 1 definition
void vMonitorTask(void *pvParameters) { monitor(); }



