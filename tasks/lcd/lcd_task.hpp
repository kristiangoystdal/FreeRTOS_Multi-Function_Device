
#ifndef __LCD_TASK_H
#define __LCD_TASK_H

#include "FreeRTOS.h"
#include <time.h>

namespace lcd_task {

  typedef enum LCDDataType {
    Timestamp,
    Alarm,
    Temperature
  } LCDDataType;

  typedef union LCDData {
    time_t xTime;
    char cAlarmLetter;
    float xTemperature;
  } LCDData;

  typedef struct {
    LCDDataType xDataType;
    LCDData xLCDData;
  } LCDMessage_t;
  
}

#endif
