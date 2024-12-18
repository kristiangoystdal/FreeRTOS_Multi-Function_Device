
#ifndef __MAX_MIN_TASK_H
#define __MAX_MIN_TASK_H

#include "max_min_task.hpp"
#include "time.h"

namespace command_task {

typedef enum CommandAction {
  DateTime,
  MinMax,
  Clock,
  Parameters,
  AlarmInfo,
  TaskState

} CommandAction;

typedef struct {
  CommandAction action;
  union {
    MaxMinMeasure_t max_min;
    time_t datetime;
  } data;
} CommandMessage_t;

void vCommandTask(void *pvParameters);

} // namespace command_task

#endif
