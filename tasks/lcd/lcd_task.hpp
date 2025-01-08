
#ifndef __LCD_TASK_H
#define __LCD_TASK_H

#include <time.h>

namespace lcd_task
{

#define LCD_CLOCK_UPDATE_TIME 33

  typedef enum LCDAction
  {
    Alarm,
    Temperature,
    BubbleLevel
  } LCDAction;

  typedef union LCDData
  {
    char cAlarmLetter;
    float xTemperature;
  } LCDData;

  typedef struct
  {
    LCDAction xAction;
    LCDData xLCDData;
  } LCDMessage_t;

  void vLCDTask(void *pvParameters);

}

#endif
