#ifndef __GLOBAL_QUEUES_H__
#define __GLOBAL_QUEUES_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

extern QueueHandle_t xQueueMaxMin;
extern QueueHandle_t xQueueAlarm;
extern QueueHandle_t xQueueLCD;
extern QueueHandle_t xQueueConfigSound;
extern QueueHandle_t xQueueTemperature;
extern QueueHandle_t xQueueRGB;
extern TaskHandle_t xBubbleLevelHandler;
extern TaskHandle_t xHitBitHandler;
void vCreateTask(TaskFunction_t pxTaskCode, const char *const pcName,
                 const uint16_t usStackDepth, void *const pvParameters,
                 UBaseType_t uxPriority, TaskHandle_t *const pxCreatedTask);
QueueHandle_t xCreateQueue(UBaseType_t uxSize, UBaseType_t uxType);

#endif // __GLOBAL_QUEUES_H__
