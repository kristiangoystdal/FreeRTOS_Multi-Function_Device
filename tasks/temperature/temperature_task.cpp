
#include "FreeRTOS.h"
#include "LM75B.h"
#include "alarm_task.hpp"
#include "configuration.hpp"
#include "date_time.hpp"
#include "lcd_task.hpp"
#include "max_min_task.hpp"
#include "queue.h"
#include "task.h"
#include <cstdio>

namespace temperature_task {

static LM75B sensor(p28, p27);

void vTemperatureTask(void *pvParameters) {
  printf("%p\n", &xQueueMaxMin);
  printf("%p\n", &xQueueAlarm);
  printf("%p\n", &xQueueLCD);
  printf("%p\n", &xQueueConsole);

  uint32_t ulNotificationValue = 0;
  if (!sensor.open()) {
    printf("Temperature sensor NOT OK\n");
  } else {
    printf("Temperature sensor OK\n");
  }
  Measure_t xMeasure;
  max_min_task::MaxMinMessage_t xMaxMinMessage;
  for (;;) {
    float xTemp = sensor.temp();
    printf("Temperature: %.2f\n", xTemp);
    time_t xMeasureTime = date_time::get_time();
    printf("Time: %d\n", xMeasureTime);
    xMeasure.xTime = xMeasureTime;
    xMeasure.xTemp = xTemp;
    xMaxMinMessage.xAction = max_min_task::Set;
    xMaxMinMessage.xMeasure = xMeasure;
    printf("%p\n", &xMaxMinMessage);
    BaseType_t xStatus = xQueueSend(xQueueMaxMin, &xMaxMinMessage, 0);
    printf("Temperature sent to MaxMin\n");
    if (xStatus == errQUEUE_FULL) {
      printf("ERROR: Queue full: Temperature -> Max/Min");
    } else {
      printf("Pass\n");
    }
    printf("LCD 0");
    lcd_task::LCDMessage_t xLCDMessage;
    printf("LCD 1");
    xLCDMessage.xAction = lcd_task::Temperature;
    printf("LCD 2");
    xLCDMessage.xLCDData.xTemperature = xTemp;
    printf("LCD 3");
    xStatus = xQueueSend(xQueueLCD, &xLCDMessage, 0);
    printf("LCD 4");

    printf("Temperature sent to LCD\n");
    if (xStatus == errQUEUE_FULL) {
      printf("ERROR: Queue full: Temperature -> LCD");
    }

    alarm_task::AlarmMessage_t xAlarmMessage;
    xAlarmMessage.xAction = alarm_task::Temp;
    xAlarmMessage.xAlarmData.xMeasure = xMeasure;
    xStatus = xQueueSend(xQueueAlarm, &xAlarmMessage, 0);
    printf("Temperature sent to Alarm\n");
    if (xStatus == errQUEUE_FULL) {
      printf("ERROR: Queue full: Temperature -> Alarm");
    }

    if (ulNotificationValue > 0) {
      // TODO: Send to console
    }
    printf("Temperature sent to console\n");

    TickType_t xPMON = configuration::xConfigGetPMON();
    xPMON = xPMON > 0 ? xPMON : portMAX_DELAY;
    ulNotificationValue = ulTaskNotifyTake(pdTRUE, xPMON);
    printf("Temperature done\n");
  }
}
} // namespace temperature_task
