
#include "alarm_task.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include "RTC.h"
#include "rgb.hpp"
#include "lcd_task.hpp"
#include "configuration.hpp"
#include "date_time.hpp"
#include <stdio.h>

namespace alarm_task {

  static AlarmInfo_t xAlarmInfo;
  static TaskHandle_t xPWMTask;
  static SemaphoreHandle_t xMutexClock;

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

  void vAlarmClockMutexInitialize() {
    xMutexClock = xSemaphoreCreateMutex();
    if(xMutexClock == NULL) {
     printf("Critical error when creating PMON's Mutex!");
    }
  }

  void vSendToLCD(QueueHandle_t xQueueLCD, char letter) {
    lcd_task::LCDMessage_t xLCDMessage;
    xLCDMessage.xDataType = lcd_task::Alarm;
    xLCDMessage.xLCDData.cAlarmLetter = letter;
    BaseType_t xStatus = xQueueSend(xQueueLCD, &xLCDMessage, 0);
    if(xStatus == errQUEUE_FULL){
      printf("ERROR: Queue full: Alarm -> LCD");
    }
  }

  void vSetRGB(float temp) {
    if(temp <= xAlarmInfo.tlow) {
      rgb::set_rgb(0, 0, 1.0);
    } else if(temp >= xAlarmInfo.thigh) {
      rgb::set_rgb(1.0, 0, 0);
    } else {
      float delta = (temp - xAlarmInfo.tlow)/(xAlarmInfo.thigh - xAlarmInfo.tlow);
      rgb::set_rgb(delta, 0, 1.0 - delta);
    }
  }

  void vClockAlarm(k) {
    xSemaphoreTake(xMutexClock, portMAX_DELAY);
    if(xAlarmInfo.clock_alarm_en) {
      xTaskNotifyGive(xPWMTask);
    }
    xSemaphoreGive(xMutexClock);
  }

  // TODO: Set clock alarm
  void vAlarmTask(void* pvParameters) {
    QueueHandle_t* pxQueueArray = (QueueHandle_t*)pvParameters;
    QueueHandle_t xQueueAlarm = (QueueHandle_t)pxQueueArray[0];
    QueueHandle_t xQueueConsole = (QueueHandle_t)pxQueueArray[1];
    QueueHandle_t xQueueLCD = (QueueHandle_t)pxQueueArray[2];
    xPWMTask = (TaskHandle_t)pxQueueArray[3];
    AlarmMessage_t xMessage;
    vAlarmInfoInitialize();
    vAlarmClockMutexInitialize();
    rgb::set_period();
    for(;;) {
      xQueueReceive(xQueueAlarm, &xMessage, portMAX_DELAY);
      switch(xMessage.xAction) {
        case Get:
          // TODO: Send to console
          break;
        case SetClock:
          xAlarmInfo.tclock = xMessage.xAlarmData.tclock;
          RTC::alarm(&vClockAlarm, *date_time::time_to_clock(xAlarmInfo.tclock));
          break;
        case SetTemp:
          xAlarmInfo.tlow = xMessage.xAlarmData.threshold.tlow;
          xAlarmInfo.thigh = xMessage.xAlarmData.threshold.thigh;
          break;
        case SetClockEn:
          xSemaphoreTake(xMutexClock, portMAX_DELAY);
          xAlarmInfo.clock_alarm_en = xMessage.xAlarmData.clock_alarm_en;
          xSemaphoreGive(xMutexClock);
          vSendToLCD(xQueueLCD, xAlarmInfo.clock_alarm_en ? 'C' : 'c');
          if(xAlarmInfo.clock_alarm_en) {
            RTC::alarmOff();
          }
          break;
        case SetTempEn:
          xAlarmInfo.temp_alarm_en = xMessage.xAlarmData.temp_alarm_en;
          vSendToLCD(xQueueLCD, xAlarmInfo.clock_alarm_en ? 'T' : 't');
          break;
        case Temp:
          float temp = xMessage.xAlarmData.xMeasure.xTemp;
          if(xAlarmInfo.temp_alarm_en && (xAlarmInfo.thigh < temp || xAlarmInfo.tlow > temp)) {
            xTaskNotifyGive(xPWMTask);
          }
          vSetRGB(temp);
          break;
        default:
          break;
      }
    }
  }
}
