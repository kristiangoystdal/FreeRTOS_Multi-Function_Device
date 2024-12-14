#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <cstdio>


// Assuming led1 and led2 are defined elsewhere in your code
extern int led1;
extern int led2;

void vTask1(void *pvParameters) {
  int32_t lValueToSend;
  BaseType_t xStatus;
  led1 = 1;
  for (;;) {
    lValueToSend = 201;
    xStatus = xQueueSend(xQueue, &lValueToSend, 0);
    monitor(); // does not return
    led1 = !led1;
  }
}

void vTask2(void *pvParameters) {
  int32_t lReceivedValue;
  BaseType_t xStatus;
  led2 = 1;
  printf("Hello from mbed -- FreeRTOS / cmd\n");
  for (;;) {
    //        vTaskDelay( 1000 );
    xStatus = xQueueReceive(xQueue, &lReceivedValue, 1000);
    if (xStatus == pdPASS) {
      printf("Received = %d", lReceivedValue);
    }
  }
}