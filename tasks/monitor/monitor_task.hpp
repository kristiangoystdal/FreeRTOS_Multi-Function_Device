#ifdef __monitor_tasks_hpp__
#define __monitor_tasks_hpp__

#include "FreeRTOS.h"
#include "queue.h"

namespace monitor_task {
void vMonitorTask(void *pvParameters);
}

#endif // __monitor_tasks_hpp__