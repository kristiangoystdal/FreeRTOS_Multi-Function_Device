
#ifndef __ALARM_TASK_H
#define __ALARM_TASK_H

#include "temperature_task.hpp"

namespace alarm_task {

  typedef enum AlarmAction {
    Get,
    Set,
    Temp
  } AlarmAction;

  typedef struct {
    AlarmAction xAction;
    temperature_task::Measure_t xMeasure;
  } AlarmMessage_t;
  
  void vAlarmTask(void* pvParameters);
}

#endif
