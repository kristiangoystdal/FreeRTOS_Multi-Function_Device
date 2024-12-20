
#include "mbed.h"
#include "alarm_task.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "rgb.hpp"
#include "lcd_task.hpp"
#include "configuration.hpp"

namespace alarm_task {

  static AlarmInfo_t xAlarmInfo;
  static AnalogIn p1(p19);
  static AnalogIn p2(p20);
  static PwmOut buzzer(p26);

  void vAlarmInfoInitialize() {
    xAlarmInfo.tlow = 10.0;
    xAlarmInfo.thigh = 30.0;
    xAlarmInfo.tclock = 0;// TODO: Fix this
    xAlarmInfo.clock_alarm_en = false;
    xAlarmInfo.temp_alarm_en = false;
  }

  void vRunAlarm(QueueHandle_t xQueueLCD, char letter) {
    TickType_t tala = configuration::xConfigGetTALA();
    buzzer.period(p1);
    buzzer = p2;
    // TODO: Disable alarm
    lcd_task::LCDMessage_t xLCDMessage;
    xLCDMessage.xDataType = lcd_task::Alarm;
    xLCDMessage.xLCDData.cAlarmLetter = letter;
    xStatus = xQueueSend(xQueueLCD, &xLCDMessage, 0);
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

  // TODO: Set clock alarm
  void vAlarmTask(void* pvParameters) {
    QueueHandle_t* pxQueueArray = (QueueHandle_t*)pvParameters;
    QueueHandle_t xQueueAlarm = (QueueHandle_t)pxQueueArray[0];
    QueueHandle_t xQueueConsole = (QueueHandle_t)pxQueueArray[1];
    QueueHandle_t xQueueLCD = (QueueHandle_t)pxQueueArray[2];
    AlarmMessage_t xMessage;
    vAlarmInfoInitialize();
    rgb::set_period();
    for(;;) {
      xQueueReceive(xQueueAlarm, &xMessage, portMAX_DELAY);
      switch(xMessage.xAction) {
        case Get:
          // TODO: Send to console
          break;
        case SetClock:
          xAlarmInfo.tclock = xMessage.xAlarmData.tclock;
          break;
        case SetTempThreshold:
          xAlarmInfo.tlow = xMessage.xAlarmData.threshold.tlow;
          xAlarmInfo.thigh = xMessage.xAlarmData.threshold.thigh;
          break;
        case SetClockEn:
          xAlarmInfo.clock_alarm_en = xMessage.xAlarmData.clock_alarm_en;
          break;
        case SetTempEn:
          xAlarmInfo.temp_alarm_en = xMessage.xAlarmData.temp_alarm_en;
          break;
        case Temp:
          float temp = xMessage.xAlarmData.xMeasure.xTemp;
          if(xAlarmInfo.temp_alarm_en && (xAlarmInfo.thigh < temp || xAlarmInfo.tlow > temp)) {
            vRunAlarm(xQueueLCD, "T");
          }
          vSetRGB(temp);
          break;
        default:
          break;
      }
    }
  }
}