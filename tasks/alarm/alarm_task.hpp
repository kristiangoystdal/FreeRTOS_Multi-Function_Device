
#ifndef __ALARM_TASK_H
#define __ALARM_TASK_H

#include <time.h>

namespace alarm_task {

  typedef struct {
    float xTemp;
    time_t xTime;
  } AlarmMessage_t;
  
}

#endif
