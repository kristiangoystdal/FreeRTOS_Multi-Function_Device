
#include "max_min_task.hpp"
#include "FreeRTOS.h"
#include "global_queues.h"
#include "queue.h"
#include "temperature_task.hpp"
#include <cfloat>
#include <stdio.h>

namespace max_min_task {

static MaxMinMeasure_t xMaxMin;

void sendMaxMin() {
  printf("Max: %f at %d\n", xMaxMin.xMax.xTemp, xMaxMin.xMax.xTime);
  // TODO: Send to console
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
