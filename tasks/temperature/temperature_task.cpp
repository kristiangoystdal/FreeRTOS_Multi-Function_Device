
#include "FreeRTOS.h"
#include "LM75B.h"
#include "alarm_task.hpp"
#include "date_time.hpp"
#include "global.h"
#include "lcd_task.hpp"
#include "max_min_task.hpp"
#include "queue.h"
#include "task.h"
#include "timers.h"
#include <cstdio>
#include "atomic.hpp"

namespace temperature_task {

static LM75B sensor(p28, p27);

static TimerHandle_t xTimer;

static atomic::Atomic<TickType_t>* xPMON;

TickType_t xConfigGetPMON() {
  return xPMON->get();
}

void vConfigSetPMON(int seconds) {
  TickType_t xTicks = pdMS_TO_TICKS(1000*seconds);
  xPMON->set(xTicks);
  if(seconds == 0) {
    if (xTimerStop(xTimer, 0) != pdPASS) {
      printf("Failed to stop timer!\n");
    }
  } else {
    if (xTimerChangePeriod(xTimer, xTicks, 0) != pdPASS) {
      printf("Failed to change timer period!\n");
    }
    if (xTimerStart(xTimer, 0) != pdPASS) {
      printf("Failed to start timer!\n");
    }
  }
}

void vTimerCallback(TimerHandle_t xTimer) {
  TemperatureData_t xMessage = false; 
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xQueueTemperature, &xMessage, &xHigherPriorityTaskWoken);
}

void vTemperatureInitializer() {
  if (!sensor.open()) {
    printf("Temperature sensor NOT OK\n");
  } else {
    printf("Temperature sensor OK\n");
  }
}

void vTemperatureTask(void *pvParameters) {
  vTemperatureInitializer();
  TemperatureData_t xToConsole = false; 
  Measure_t xMeasure;
  max_min_task::MaxMinMessage_t xMaxMinMessage;
  xPMON = new atomic::Atomic<TickType_t>(pdMS_TO_TICKS(1000*PMON_DEFAULT_VALUE));
  xTimer = xTimerCreate("Temperature Timer", pdMS_TO_TICKS(1000*PMON_DEFAULT_VALUE), pdTRUE, (void *)1, vTimerCallback);
  for (;;) {
    float xTemp = sensor.temp();
    time_t xMeasureTime = date_time::get_time();
    xMeasure.xTime = xMeasureTime;
    xMeasure.xTemp = xTemp;
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

    if (xToConsole == true) {
      printf("Temperature: %.1f\n", xTemp);
    }
    xQueueReceive(xQueueTemperature, &xToConsole, portMAX_DELAY);
  }
}
} // namespace temperature_task
