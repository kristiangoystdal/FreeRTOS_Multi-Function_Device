#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <cstdio>
#include <stdio.h>

// Assuming led1 and led2 are defined elsewhere in your code
extern int led1;
extern int led2;

// Function prototype for the monitor function
extern void monitor(void);

// Task 1 definition
void vTask1(void *pvParameters) {
  // Cast pvParameters to QueueHandle_t
  QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
  int32_t lValueToSend;
  BaseType_t xStatus;
  led1 = 1;
  for (;;) {
    lValueToSend = 201;
    // Send to the queue
    xStatus = xQueueSend(xQueue, &lValueToSend, 0);
    if (xStatus == pdPASS) {
      monitor(); // Assuming this does not return
    }
    led1 = !led1; // Toggle led1
  }
}

// Task 2 definition
void vTask2(void *pvParameters) {
  // Cast pvParameters to QueueHandle_t
  QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
  int32_t lReceivedValue;
  BaseType_t xStatus;
  led2 = 1;
  printf("Hello from mbed -- FreeRTOS / cmd\n");
  for (;;) {
    // Receive from the queue
    xStatus = xQueueReceive(xQueue, &lReceivedValue, pdMS_TO_TICKS(1000));
    if (xStatus == pdPASS) {
      printf("Received = %d\n", lReceivedValue);
    }
  }
}

// Task Scheduler initialization
void init_TaskScheduler(QueueHandle_t *xQueue) {
  // Create the queue
  *xQueue = xQueueCreate(3, sizeof(int32_t));
  if (*xQueue == NULL) {
    printf("Failed to create the queue\n");
    return;
  }
}
