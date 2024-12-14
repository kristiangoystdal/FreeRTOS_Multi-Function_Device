#ifndef __CMD_H
#define __CMD_H

#include "FreeRTOS.h"
#include "queue.h"

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

void init_TaskScheduler(QueueHandle_t *);

#endif
