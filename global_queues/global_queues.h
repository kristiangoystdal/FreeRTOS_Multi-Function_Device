#ifndef __GLOBAL_QUEUES_H__
#define __GLOBAL_QUEUES_H__

#include "FreeRTOS.h"
#include "queue.h"

QueueHandle_t xQueueMaxMin;
QueueHandle_t xQueueAlarm;
QueueHandle_t xQueueLCD;
QueueHandle_t xQueueConsole;

#endif // __GLOBAL_QUEUES_H__
