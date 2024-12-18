
#include "max_min_task.hpp"
#include "FreeRTOS.h"
#include "comando.hpp"
#include "command_task.hpp"
#include "queue.h"
#include "temperature_task.hpp"
#include <stdio.h>

namespace max_min_task {

static MaxMinMeasure_t xMaxMin;

void sendMaxMin(QueueHandle_t xQueueConsole) {
  // TODO: Send to console
  command_task::CommandMessage_t xMessage;
  xMessage.action = command_task::MinMax;
  xMessage.data.max_min = xMaxMin;

  printf("Sending Max/Min\n");
  xQueueSend(xQueueConsole, &xMessage, 0);
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
  xMaxMin.xMax.xTemp = -100;
  xMaxMin.xMin.xTime = 0;
  xMaxMin.xMin.xTemp = 100;
}

void vMaxMinTask(void *pvParameters) {
  QueueHandle_t *pxQueueArray = (QueueHandle_t *)pvParameters;
  QueueHandle_t xQueueMaxMin = (QueueHandle_t)pxQueueArray[0];
  QueueHandle_t xQueueConsole = (QueueHandle_t)pxQueueArray[1];
  MaxMinMessage_t xMessage;
  for (;;) {
    xQueueReceive(xQueueMaxMin, &xMessage, portMAX_DELAY);
    printf("R");

    if (xMessage.xAction == Get) {
      sendMaxMin(xQueueConsole);
    } else {
      updateMaxMin(xMessage.xMeasure);
    }
  }
}

} // namespace max_min_task
