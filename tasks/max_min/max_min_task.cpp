
#include "max_min_task.hpp"
#include <cfloat>
#include "FreeRTOS.h"
#include "queue.h"
#include "temperature_task.hpp"

namespace max_min_task {

  static MaxMinMeasure_t xMaxMin;

  void sendMaxMin(QueueHandle_t xQueueConsole) {
    // TODO: Send to console
  }

  void updateMaxMin(temperature_task::Measure_t xMeasure) {
    if(xMaxMin.xMax.xTemp < xMeasure.xTemp) {
      xMaxMin.xMax.xTime = xMeasure.xTime;
      xMaxMin.xMax.xTemp = xMeasure.xTemp;
    }

    if(xMaxMin.xMin.xTemp > xMeasure.xTemp) {
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

  void vMaxMinTask(void* pvParameters) {
    QueueHandle_t* pxQueueArray = (QueueHandle_t*)pvParameters;
    QueueHandle_t xQueueMaxMin = (QueueHandle_t)pxQueueArray[0];
    QueueHandle_t xQueueConsole = (QueueHandle_t)pxQueueArray[1];
    MaxMinMessage_t xMessage;
    for(;;) {
      xQueueReceive(xQueueMaxMin, &xMessage, portMAX_DELAY);
      if(xMessage.xAction == Get) {
        sendMaxMin(xQueueConsole);
      } else {
        updateMaxMin(xMessage.xMeasure);
      }
    }
  }

}
