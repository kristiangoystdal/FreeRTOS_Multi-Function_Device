
#include "global.h"
#include <stdio.h>

// Define the global queues
QueueHandle_t xQueueMaxMin;
QueueHandle_t xQueueAlarm;
QueueHandle_t xQueueLCD;
QueueHandle_t xQueueConfigSound;
QueueHandle_t xQueueTemperature;
QueueHandle_t xQueueRGB;
TaskHandle_t xBubbleLevelHandler;
TaskHandle_t xHitBitHandler;

void vCreateTask(TaskFunction_t pxTaskCode, const char *const pcName,
                 const uint16_t usStackDepth, void *const pvParameters,
                 UBaseType_t uxPriority, TaskHandle_t *const pxCreatedTask) {
  BaseType_t xReturn = xTaskCreate(pxTaskCode, pcName, usStackDepth,
                                   pvParameters, uxPriority, pxCreatedTask);
  if (xReturn == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY) {
    printf("Failed to create the task %s\n", pcName);
  }
}

QueueHandle_t xCreateQueue(UBaseType_t uxSize, UBaseType_t uxType) {
  QueueHandle_t xQueue = xQueueCreate(uxSize, uxType);
  if (xQueue == NULL) {
    printf("Failed to create the queue\n");
  }
  return xQueue;
}

