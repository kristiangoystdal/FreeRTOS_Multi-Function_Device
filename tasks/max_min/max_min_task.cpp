
#include "max_min_task.hpp"
#include <cfloat>
#include "FreeRTOS.h"
#include "queue.h"

namespace max_min_task {

  static MaxMinMeasure_t xMaxMin;

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
      if(xMessage.action == Get) {
        sendMaxMin(xQueueConsole);
      } else {
        updateMaxMin(xMessage);
      }
    }
  }

  void sendMaxMin(QueueHandle_t xQueueConsole) {
    // TODO: Send to console
  }

  void updateMaxMin(MaxMinMessage_t xMessage) {
    if(xMaxMin.xMax.xTemp < xMessage.xTemp) {
      xMaxMin.xMax.xTime = xMessage.xTime;
      xMaxMin.xMax.xTemp = xMessage.xTemp;
    }

    if(xMaxMin.xMin.xTemp > xMessage.xTemp) {
      xMaxMin.xMin.xTime = xMessage.xTime;
      xMaxMin.xMin.xTemp = xMessage.xTemp;
    }
  }

}