
#ifndef __GLOBAL_STRUCTURES_H
#define __GLOBAL_STRUCTURES_H

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

QueueHandle_t xQueue;
QueueHandle_t xQueueMaxMin;
QueueHandle_t xQueueAlarm;
QueueHandle_t xQueueLCD;
TaskHandle_t xTaskTemperatureHandle;

#endif
