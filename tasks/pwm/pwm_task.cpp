
#include "pwm_task.hpp"
#include "mbed.h"
#include "FreeRTOS.h"
#include "task.h"
#include "configuration.hpp"

namespace pwm_task {

  static AnalogIn p1(p19);
  static AnalogIn p2(p20);
  static PwmOut buzzer(p26);

  void vPWMTask(void* pvParameters) {
    for(;;) {
      TickType_t xTALA = configuration::xConfigGetTALA();
      uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xTALA);
      if(ulNotificationValue > 0) {
        buzzer.period(p1);
        buzzer = p2;
      } else {
        buzzer = 0;
      }
    }
  }

}
