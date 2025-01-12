
#ifndef __TEMPERATURE_TASK_H
#define __TEMPERATURE_TASK_H

#include <time.h>
#include "FreeRTOS.h"

namespace temperature_task {

#define PMON_DEFAULT_VALUE 3

typedef struct {
  float xTemp;
  time_t xTime;
} Measure_t;

typedef bool TemperatureMessage_t;

void vTemperatureTask(void *pvParameters);

TickType_t xConfigGetPMON();
void vConfigSetPMON(int seconds);

} // namespace temperature_task

#endif
