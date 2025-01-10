
#include "bubble_level_task.hpp"
#include "FreeRTOS.h"
#include "MMA7660.h"
#include "atomic.hpp"
#include "lcd_task.hpp"
#include "task.h"
#include <stdio.h>
#include "global_queues.h"

namespace bubble_level_task {

  static MMA7660 MMA(p28, p27); //I2C Accelerometer

  static TaskHandle_t xHandle;
  static atomic::Atomic<bool>* xBubbleLevelEnabled;

  bool xGetBubbleLevelEnabled() {
    return xBubbleLevelEnabled->get();
  }

  void vSetBubbleLevelEnabled(bool enabled) {
    xBubbleLevelEnabled->set(enabled);
    if(enabled) {
      vTaskResume(xHandle);
    } else {
      vTaskSuspend(xHandle);
    }
  }

  void vBubbleLevelTask(void* pvParameters) {
    printf("Bubble Level Task\n");
    lcd_task::LCDMessage_t xMessage;
    xHandle = xTaskGetCurrentTaskHandle();
    xBubbleLevelEnabled = new atomic::Atomic<bool>(true);
    TickType_t xTicks = pdMS_TO_TICKS(BUBBLE_LEVEL_UPDATE_TIME);
    if(!MMA.testConnection()){
      printf("Accelerometer NOT OK");
    }
    int bubble_x, bubble_y;
    for(;;) {
      bubble_x = (bubble_x + MMA.x() * 32.0)/2.0;
      bubble_y = (bubble_y -(MMA.y() * 16.0))/2.0;
      xMessage.xAction = lcd_task::BubbleLevel;
      xMessage.xLCDData.xBubbleLevelPos.x = bubble_x;
      xMessage.xLCDData.xBubbleLevelPos.y = bubble_y;
      BaseType_t xStatus = xQueueSend(xQueueLCD, &xMessage, 0);
      if(xStatus == errQUEUE_FULL){
        printf("ERROR: Queue full: Bubble Level -> LCD");
      }
      vTaskDelay(xTicks);
    }
  }

}
