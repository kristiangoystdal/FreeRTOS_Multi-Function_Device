#include "command_task.hpp"
#include "FreeRTOS.h"
#include "global_queues.h"
#include "queue.h"
#include <cfloat>
#include <cstdio>
#include <stdio.h>

namespace command_task {

void vCommandTask(void *pvParameters) {
  printf("Command Task\n");
  CommandMessage_t xMessage;
  for (;;) {
    printf("CMD\n");
    xQueueReceive(xQueueConsole, &xMessage, portMAX_DELAY);
    switch (xMessage.action) {
    case DateTime:
      // Placeholder for command
      printf("cmd_rp\n");
      break;
    case MinMax:
      // Placeholder for command
      printf("cmd_rmm\n");
      printf("Max temperature at %d: %f\n", xMessage.data.max_min.xMax.xTime,
             xMessage.data.max_min.xMax.xTemp);
      printf("Min temperature at %d: %f\n", xMessage.data.max_min.xMin.xTime,
             xMessage.data.max_min.xMin.xTemp);
      break;
    case Clock:
      // Placeholder for command
      printf("cmd_rc 1\n");
      break;
    case Parameters:
      // Placeholder for command
      printf("cmd_cmm\n");
      break;
    case AlarmInfo:
      // Placeholder for command
      printf("cmd_rai\n");
      break;
    case TaskState:
      // Placeholder for command
      printf("cmd_rts\n");
      break;
    default:
      break;
    }
  }
}

} // namespace command_task
