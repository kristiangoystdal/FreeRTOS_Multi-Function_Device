
#ifndef __MAX_MIN_TASK_H
#define __MAX_MIN_TASK_H

#include "time.h"

namespace max_min_task {

  typedef enum MaxMinAction {
    Get,
    Set
  } MaxMinAction;

  typedef struct {
    MaxMinAction action;
    float xTemp;
    time_t xTime;
  } MaxMinMessage_t;

  typedef struct {
    float xTemp;
    time_t xTime;
  } Measure_t;

  typedef struct {
    Measure_t xMax;
    Measure_t xMin;
  } MaxMinMeasure_t;
  
  void vMaxMinInitialize(void);
  void vMaxMinTask(void* pvParameters);
}

#endif
