
#include "lcd_task.hpp"
#include "FreeRTOS.h"
#include "LCD.h"
#include "RTC.h"
#include "date_time.hpp"
#include "global.h"
#include "queue.h"
#include <stdio.h>


namespace lcd_task {

void vLCDInitialize() {
  setup_lcd();
  write_clock_alarm(false);
  write_temp_alarm(false);
}

void vUpdateClockISR() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  LCDMessage_t xMessage;
  xMessage.xAction = Clock;
  xMessage.xLCDData.time = date_time::get_time();
  xQueueSendFromISR(xQueueLCD, &xMessage, &xHigherPriorityTaskWoken);
}

void vLCDTask(void *pvParameters) {
  printf("LCD Task\n");
  LCDMessage_t xMessage;
  vLCDInitialize();
  for (;;) {
    BaseType_t xStatus = xQueueReceive(xQueueLCD, &xMessage, portMAX_DELAY);
    switch (xMessage.xAction) {
    case Alarm:
      char cAlarmLetter = xMessage.xLCDData.cAlarmLetter;
      switch (cAlarmLetter) {
      case 'c':
        write_clock_alarm(false);
        break;
      case 'C':
        write_clock_alarm(true);
        break;
      case 't':
        write_temp_alarm(false);
        break;
      case 'T':
        write_temp_alarm(true);
        break;
      default:
        break;
      }
      break;
    case Temperature:
      float xTemperature = xMessage.xLCDData.xTemperature;
      write_temperature(xTemperature);
      break;
    case Clock:
      time_t time = xMessage.xLCDData.time;
      tm* t = date_time::time_to_clock(time);
      write_time(t->tm_hour, t->tm_min, t->tm_sec);
      break;
    case BubbleLevel:
      int x = xMessage.xLCDData.xBubbleLevelPos.x;
      int y = xMessage.xLCDData.xBubbleLevelPos.y;
      draw_bubble_level(x, y);
      break;
    default:
      break;
    }
  }
}
} // namespace lcd_task
