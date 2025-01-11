
#include "max_min_task.hpp"
#include "FreeRTOS.h"
#include "date_time.hpp"
#include "global_queues.h"
#include "queue.h"
#include "temperature_task.hpp"
#include <cfloat>
#include <stdio.h>

namespace max_min_task {

static MaxMinMeasure_t xMaxMin;

void sendMaxMin() {
  char buffer[100];
  date_time::convertTimeToString(xMaxMin.xMax.xTime, buffer, sizeof(buffer));
  printf("Max: %.1f at %s\n", xMaxMin.xMax.xTemp, buffer);
  date_time::convertTimeToString(xMaxMin.xMin.xTime, buffer, sizeof(buffer));
  printf("Min: %.1f at %s\n", xMaxMin.xMin.xTemp, buffer);
}

void updateMaxMin(temperature_task::Measure_t xMeasure) {
  if (xMaxMin.xMax.xTemp < xMeasure.xTemp) {
    xMaxMin.xMax.xTime = xMeasure.xTime;
    xMaxMin.xMax.xTemp = xMeasure.xTemp;
  }

  if (xMaxMin.xMin.xTemp > xMeasure.xTemp) {
    xMaxMin.xMin.xTime = xMeasure.xTime;
    xMaxMin.xMin.xTemp = xMeasure.xTemp;
  }
}

void vMaxMinInitialize(void) {
  xMaxMin.xMax.xTime = 0;
  xMaxMin.xMax.xTemp = -FLT_MAX;
  xMaxMin.xMin.xTime = 0;
  xMaxMin.xMin.xTemp = FLT_MAX;
}

void vMaxMinTask(void *pvParameters) {
  printf("MaxMin Task\n");
  MaxMinMessage_t xMessage;
  vMaxMinInitialize();
  for (;;) {
    xQueueReceive(xQueueMaxMin, &xMessage, portMAX_DELAY);
    if (xMessage.xAction == Get) {
      sendMaxMin();
    } else {
      updateMaxMin(xMessage.xMeasure);
    }
  }
}

} // namespace max_min_task
