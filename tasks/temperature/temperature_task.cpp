
#include "global_structures.h"
#include "configuration.hpp"
#include "LM75B.h"
#include <time.h>
#include "alarm_task.hpp"

namespace temperature_task {

  LM75B sensor(p28,p27);
  
  void vReadTemperature(void* pvParameters) {
    for(;;) {
      TickType_t xPMON = configuration::xConfigGetPMON();
      xPMON = xPMON > 0 ? xPMON : portMAX_DELAY;
      uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xPMON);

      float xTemp = sensor.temp();
      time_t xMeasureTime = time(NULL);

      max_min_task::MaxMinMessage_t xMaxMinMessage;
      xMaxMinMessage.xTime = xMeasureTime;
      xMaxMinMessage.xTemp = xTemp;
      BaseType_t xStatus = xQueueSend(xQueueMaxMin, &xTemp, 0);
      if(xStatus == errQUEUE_FULL){
        printf("ERROR: Queue full: Temperature -> Max/Min");
      }

      lcd_task::LCDMessage_t xLCDMessage;
      xLCDMessage.xDataType = lcd_task::LCDDataType::Temperature;
      xLCDMessage.xLCDData = xTemp;
      BaseType_t xStatus = xQueueSend(xQueueLCD, &xTemp, 0);
      if(xStatus == errQUEUE_FULL){
        printf("ERROR: Queue full: Temperature -> LCD");
      }

      alarm_task::AlarmMessage_t xAlarmMessage;
      xAlarmMessage.xTime = xMeasureTime;
      xAlarmMessage.xTemp = xTemp;
      BaseType_t xStatus = xQueueSend(xQueueAlarm, &xTemp, 0);
      if(xStatus == errQUEUE_FULL){
        printf("ERROR: Queue full: Temperature -> Alarm");
      }
    }
  }
}