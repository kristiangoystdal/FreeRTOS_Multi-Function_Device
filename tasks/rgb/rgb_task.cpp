
#include "rgb_task.hpp"
#include "rgb.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "global.h"

namespace rgb_task {

static float red = 0.0;
static float green = 0.0;
static float blue = 0.0;

void vRGBInitializer() {
  rgb::set_period();
}

void vRGBTask(void *pvParameters) {
  RGBMessage_t xMessage;
  for(;;) {
    xQueueReceive(xQueueRGB, &xMessage, portMAX_DELAY);
    switch (xMessage.xAction) {
      case Set:
        red = xMessage.xData.r;
        green = xMessage.xData.g;
        blue = xMessage.xData.b;
        rgb::set_rgb(red, green, blue);
        break;
      case Update:
        rgb::set_rgb(red, green, blue);
        break;
      default:
        break;
    }
  }
  
}

} // namespace rgb_task
