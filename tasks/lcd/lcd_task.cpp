
#include "lcd_task.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "LCD.h"
#include "RTC.h"
#include "date_time.hpp"

namespace lcd_task {

  void vUpdateClock() {
    tm* tm_ = date_time::get_time_tm();
    write_time(tm_->tm_hour, tm_->tm_min, tm_->tm_sec);
  }

  void vLCDInitialize() {
    setup_lcd();
    draw_bubble_level();
    write_clock_alarm(false);
    write_temp_alarm(false);
  }

  void vLCDTask(void* pvParameters) {
    QueueHandle_t* pxQueueArray = (QueueHandle_t*)pvParameters;
    QueueHandle_t xQueueLCD = (QueueHandle_t)pxQueueArray[0];
    LCDMessage_t xMessage;
    vLCDInitialize();
    for(;;) {
      TickType_t xTicks = pdMS_TO_TICKS(LCD_CLOCK_UPDATE_TIME);
      xQueueReceive(xQueueLCD, &xMessage, xTicks);
      switch(xMessage.xAction) {
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
          int xTemperature = xMessage.xLCDData.xTemperature;
          write_temperature(xTemperature);
          break;
        default:
          break;
      }
      // Always update clock and bubble level
      vUpdateClock();
      draw_bubble_level();
    }
  }
}
