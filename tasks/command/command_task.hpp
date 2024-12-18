
#ifndef __MAX_MIN_TASK_H
#define __MAX_MIN_TASK_H

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

} CommandMessage_t;



void vCommandTask(void* pvParameters);

} // namespace command_task

#endif
