#ifndef __GLOBAL_QUEUES_H__
#define __GLOBAL_QUEUES_H__

#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t xQueueMaxMin;
extern QueueHandle_t xQueueAlarm;
extern QueueHandle_t xQueueLCD;

#endif // __GLOBAL_QUEUES_H__
