#ifndef __GLOBAL_QUEUES_H__
#define __GLOBAL_QUEUES_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

extern QueueHandle_t xQueueMaxMin;
extern QueueHandle_t xQueueAlarm;
extern QueueHandle_t xQueueLCD;
extern TaskHandle_t xPWMHandler;
extern TaskHandle_t xTemperatureHandler;

#endif // __GLOBAL_QUEUES_H__
