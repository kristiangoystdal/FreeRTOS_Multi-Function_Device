
#ifndef __TEMPERATURE_TASK_H
#define __TEMPERATURE_TASK_H

#include <time.h>

namespace temperature_task {

typedef struct {
  float xTemp;
  time_t xTime;
} Measure_t;

void vTemperatureTask(void *pvParameters);

void get_temperature(float *temp);
} // namespace temperature_task

#endif
