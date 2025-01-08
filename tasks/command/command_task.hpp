
#ifndef __COMMAND_TASK_H
#define __COMMAND_TASK_H

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
    max_min_task::MaxMinMeasure_t max_min;
  } data;
} CommandMessage_t;

void vCommandTask(void *pvParameters);

} // namespace command_task

#endif
