#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <cstdio>
#include <stdio.h>

// Function prototype for the monitor function
extern void monitor(void);

// Monitor definition
void vMonitorTask(void *pvParameters) { monitor(); }



