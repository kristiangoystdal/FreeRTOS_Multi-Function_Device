
#ifndef __LCD_TASK_H
#define __LCD_TASK_H

#include "FreeRTOS.h"
#include <time.h>

namespace lcd_task {

  typedef enum class LCDDataType {
    Timestamp,
    Alarm,
    Temperature
  };

  typedef union LCDData {
    time_t xTime;
    char cAlarmLetter;
    float xTemperature;
  };

  typedef struct {
    BaseType_t xDataType;
    LCDData xLCDData;
  } LCDMessage_t;
  
}

#endif
