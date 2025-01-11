
#ifndef __LCD_TASK_H
#define __LCD_TASK_H

#include <time.h>

namespace lcd_task
{

  typedef enum LCDAction
  {
    Alarm,
    Temperature,
    Clock,
    BubbleLevel
  } LCDAction;

  typedef struct
  {
    int x;
    int y;
  } Pos;

  typedef union LCDData
  {
    char cAlarmLetter;
    float xTemperature;
    time_t time;
    Pos xBubbleLevelPos;
  } LCDData;

  typedef struct
  {
    LCDAction xAction;
    LCDData xLCDData;
  } LCDMessage_t;

  void vLCDTask(void *pvParameters);
  void vUpdateClockISR();

}

#endif
