
#include "FreeRTOS.h"
#include "LM75B.h"
#include "alarm_task.hpp"
#include "configuration.hpp"
#include "date_time.hpp"
#include "lcd_task.hpp"
#include "max_min_task.hpp"
#include "queue.h"
#include "task.h"


namespace temperature_task {

static LM75B sensor(p28, p27);

void vTemperatureTask(void *pvParameters) {
  QueueHandle_t *pxQueueArray = (QueueHandle_t *)pvParameters;
  QueueHandle_t xQueueMaxMin = (QueueHandle_t)pxQueueArray[0];
  QueueHandle_t xQueueAlarm = (QueueHandle_t)pxQueueArray[1];
  QueueHandle_t xQueueLCD = (QueueHandle_t)pxQueueArray[2];
  QueueHandle_t xQueueConsole = (QueueHandle_t)pxQueueArray[3];
  uint32_t ulNotificationValue = 0;
  if (!sensor.open()) {
    printf("Temperature sensor NOT OK\n");
  } else {
    printf("Temperature sensor OK\n");
  }
  for (;;) {
    float xTemp = sensor.temp();
    time_t xMeasureTime = date_time::get_time();

    Measure_t xMeasure;
    xMeasure.xTime = xMeasureTime;
    xMeasure.xTemp = xTemp;
    max_min_task::MaxMinMessage_t xMaxMinMessage;
    xMaxMinMessage.xAction = max_min_task::Set;
    xMaxMinMessage.xMeasure = xMeasure;
    BaseType_t xStatus = xQueueSend(xQueueMaxMin, &xMaxMinMessage, 0);
    if (xStatus == errQUEUE_FULL) {
      printf("ERROR: Queue full: Temperature -> Max/Min");
    }

    lcd_task::LCDMessage_t xLCDMessage;
    xLCDMessage.xAction = lcd_task::Temperature;
    xLCDMessage.xLCDData.xTemperature = xTemp;
    xStatus = xQueueSend(xQueueLCD, &xLCDMessage, 0);
    if (xStatus == errQUEUE_FULL) {
      printf("ERROR: Queue full: Temperature -> LCD");
    }

    alarm_task::AlarmMessage_t xAlarmMessage;
    xAlarmMessage.xAction = alarm_task::Temp;
    xAlarmMessage.xAlarmData.xMeasure = xMeasure;
    xStatus = xQueueSend(xQueueAlarm, &xAlarmMessage, 0);
    if (xStatus == errQUEUE_FULL) {
      printf("ERROR: Queue full: Temperature -> Alarm");
    }

    if (ulNotificationValue > 0) {
      // TODO: Send to console
    }
    TickType_t xPMON = configuration::xConfigGetPMON();
    xPMON = xPMON > 0 ? xPMON : portMAX_DELAY;
    ulNotificationValue = ulTaskNotifyTake(pdTRUE, xPMON);
  }
}
} // namespace temperature_task
