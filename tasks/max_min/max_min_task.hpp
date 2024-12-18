
#ifndef __MAX_MIN_TASK_H
#define __MAX_MIN_TASK_H

#include "temperature_task.hpp"

namespace max_min_task {

  typedef enum MaxMinAction {
    Get,
    Set
  } MaxMinAction;

  typedef struct {
    MaxMinAction xAction;
    temperature_task::Measure_t xMeasure;
  } MaxMinMessage_t;

  typedef struct {
    temperature_task::Measure_t xMax;
    temperature_task::Measure_t xMin;
  } MaxMinMeasure_t;
  
  void vMaxMinInitialize(void);
  void vMaxMinTask(void* pvParameters);
}

#endif
