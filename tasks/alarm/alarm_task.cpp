
#include "alarm_task.hpp"
#include "FreeRTOS.h"
#include "RTC.h"
#include "config_sound_task.hpp"
#include "date_time.hpp"
#include "global.h"
#include "lcd_task.hpp"
#include "queue.h"
#include "rgb_task.hpp"
#include "semphr.h"
#include "task.h"
#include <stdio.h>

namespace alarm_task {

static AlarmInfo_t xAlarmInfo;

void vAlarmInfoInitialize() {
  xAlarmInfo.tlow = 10.0;
  xAlarmInfo.thigh = 30.0;
  tm *t = date_time::get_time_tm();
  t->tm_hour = 12;
  t->tm_min = 0;
  t->tm_sec = 0;
  xAlarmInfo.tclock = date_time::clock_to_time(t);
  xAlarmInfo.clock_alarm_en = false;
  xAlarmInfo.temp_alarm_en = false;
}

void vSendToLCD(QueueHandle_t xQueueLCD, char letter) {
  lcd_task::LCDMessage_t xLCDMessage;
  xLCDMessage.xAction = lcd_task::Alarm;
  xLCDMessage.xLCDData.cAlarmLetter = letter;
  BaseType_t xStatus = xQueueSend(xQueueLCD, &xLCDMessage, 0);
  if (xStatus == errQUEUE_FULL) {
    printf("ERROR: Queue full: Alarm -> LCD");
  }
}

void vSetRGB(float temp) {
  rgb_task::RGBMessage_t xRGBMessage;
  xRGBMessage.xAction = rgb_task::Set;
  xRGBMessage.xData.g = 0.0;
  if (temp <= xAlarmInfo.tlow) {
    xRGBMessage.xData.r = 0.0;
    xRGBMessage.xData.b = 1.0;
  } else if (temp >= xAlarmInfo.thigh) {
    xRGBMessage.xData.r = 1.0;
    xRGBMessage.xData.b = 0.0;
  } else {
    float delta =
        (temp - xAlarmInfo.tlow) / (xAlarmInfo.thigh - xAlarmInfo.tlow);
    xRGBMessage.xData.r = delta;
    xRGBMessage.xData.b = 1.0 - delta;
  }
  if (xQueueSend(xQueueRGB, &xRGBMessage, 0) == errQUEUE_FULL) {
    printf("ERROR: Queue full: Alarm -> RGB");
  }
}

void vClockAlarm() {
  config_sound_task::ConfigSoundMessage_t xMessage;
  xMessage.xAction = config_sound_task::Alarm;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xQueueConfigSound, &xMessage, &xHigherPriorityTaskWoken);
}

void vAlarmTask(void *pvParameters) {
  printf("Alarm Task\n");
  AlarmMessage_t xMessage;
  config_sound_task::ConfigSoundMessage_t xConfigSoundMessage;
  xConfigSoundMessage.xAction = config_sound_task::Alarm;
  vAlarmInfoInitialize();
  for (;;) {
    xQueueReceive(xQueueAlarm, &xMessage, portMAX_DELAY);
    switch (xMessage.xAction) {
    case Get:
      char time_string[9];
      struct tm *time_info;
      time_info = localtime(&xAlarmInfo.tclock);
      strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);
      printf("Alarm Clock: %s\n", time_string);
      printf("Low temperature theshold: %.1f\n", xAlarmInfo.tlow);
      printf("High temperature theshold: %.1f\n", xAlarmInfo.thigh);
      printf("Clock Alarm enabled: %s\n",
             xAlarmInfo.clock_alarm_en ? "Yes" : "No");
      printf("Temperature Alarm enabled: %s\n",
             xAlarmInfo.temp_alarm_en ? "Yes" : "No");
      break;
    case SetClock:
      xAlarmInfo.tclock = xMessage.xAlarmData.tclock;
      if (!xAlarmInfo.clock_alarm_en) {
        RTC::alarm(&vClockAlarm, *date_time::time_to_clock(xAlarmInfo.tclock));
      }
      break;
    case SetTemp:
      xAlarmInfo.tlow = xMessage.xAlarmData.threshold.tlow;
      xAlarmInfo.thigh = xMessage.xAlarmData.threshold.thigh;
      break;
    case SetClockEn:
      xAlarmInfo.clock_alarm_en = xMessage.xAlarmData.clock_alarm_en;
      vSendToLCD(xQueueLCD, xAlarmInfo.clock_alarm_en ? 'C' : 'c');
      if (xAlarmInfo.clock_alarm_en) {
        RTC::alarm(&vClockAlarm, *date_time::time_to_clock(xAlarmInfo.tclock));
      } else {
        RTC::alarmOff();
      }
      break;
    case SetTempEn:
      xAlarmInfo.temp_alarm_en = xMessage.xAlarmData.temp_alarm_en;
      vSendToLCD(xQueueLCD, xAlarmInfo.temp_alarm_en ? 'T' : 't');
      break;
    case Temp:
      float temp = xMessage.xAlarmData.xMeasure.xTemp;
      if (xAlarmInfo.temp_alarm_en &&
          (xAlarmInfo.thigh < temp || xAlarmInfo.tlow > temp)) {
        if (xQueueSend(xQueueConfigSound, &xConfigSoundMessage, 0) == errQUEUE_FULL) {
          printf("ERROR: Queue full: Alarm -> Config Sound");
        }
      }
      vSetRGB(temp);
      break;
    default:
      break;
    }
  }
}
} // namespace alarm_task
