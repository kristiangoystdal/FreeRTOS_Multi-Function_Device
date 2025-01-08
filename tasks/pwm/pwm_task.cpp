
#include "pwm_task.hpp"
#include "mbed.h"
#include "FreeRTOS.h"
#include "task.h"
#include "configuration.hpp"
#include "atomic.hpp"

namespace pwm_task {

  static AnalogIn p1(p19);
  static AnalogIn p2(p20);
  static PwmOut buzzer(p26);

  static float fPeriod = 0.0;
  static float fDutyCycle = 0.0;

  static atomic::Atomic<bool> xConfigSoundEnabled;

  bool xGetConfigSoundEnabled() {
    return xConfigSoundEnabled.get();
  }

  void vSetConfigSoundEnabled(bool enabled) {
    xConfigSoundEnabled.set(enabled);
  }

  void vPWMTask(void* pvParameters) {
    xConfigSoundEnabled = new atomic::Atomic<bool>(false);
    for(;;) {
      TickType_t xTALA = configuration::xConfigGetTALA();
      uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xTALA);
      if(xGetConfigSoundEnabled()) {
        fPeriod = p1;
        fDutyCycle = p2;
      }
      if(ulNotificationValue > 0) {
        buzzer.period(fPeriod);
        buzzer = fDutyCycle;
      } else {
        buzzer = 0.0;
      }
    }
  }

}
