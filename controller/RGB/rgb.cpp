
#include "rgb.hpp"
#include "mbed.h"
#include "FreeRTOS.h"
#include "semphr.h"

namespace rgb {

  static PwmOut r (p23);
  static PwmOut g (p24);
  static PwmOut b (p25);

  static float red_;
  static float green_;
  static float blue_;

  static SemaphoreHandle_t xMutex;

  void set_rgb(float red, float green, float blue) {
    xSemaphoreTake(xMutex, portMAX_DELAY);
    r = 1.0 - red;
    g = 1.0 - green;
    b = 1.0 - blue;
    red_ = red;
    green_ = green;
    blue_ = blue;
    xSemaphoreGive(xMutex);
  }

  void refresh_rgb() {
    xSemaphoreTake(xMutex, portMAX_DELAY);
    r = 1.0 - red_;
    g = 1.0 - green_;
    b = 1.0 - blue_;
    xSemaphoreGive(xMutex);
  }

  void rgb_init() {
    xMutex = xSemaphoreCreateMutex();
    r.period(0.001);
    red_ = 0.0;
    green_ = 0.0;
    blue_ = 0.0;
  }

}