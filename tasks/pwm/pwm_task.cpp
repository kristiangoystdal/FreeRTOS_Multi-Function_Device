
#include "pwm_task.hpp"
#include "FreeRTOS.h"
#include "atomic.hpp"
#include "configuration.hpp"
#include "mbed.h"
#include "portmacro.h"
#include "task.h"

namespace pwm_task {

static AnalogIn p1(p19);
static AnalogIn p2(p20);
static PwmOut buzzer(p26);

static float fPeriod = 0.0;
static float fDutyCycle = 0.0;

const float min_output = 0.001;
const float max_output = 0.003;

static atomic::Atomic<bool> *xConfigSoundEnabled;

bool xGetConfigSoundEnabled() { return xConfigSoundEnabled->get(); }

void vSetConfigSoundEnabled(bool enabled) { xConfigSoundEnabled->set(enabled); }

void vPWMTask(void *pvParameters) {
  printf("PWM Task\n");
  xConfigSoundEnabled = new atomic::Atomic<bool>(false);
  bool xEnabled = false;
  for (;;) {
    TickType_t xTALA =
        xEnabled ? configuration::xConfigGetTALA() : portMAX_DELAY;
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xTALA);
    if (xGetConfigSoundEnabled()) {
      fPeriod = min_output + (max_output - min_output) * p1;
      fDutyCycle = p2;
    }

    if (ulNotificationValue > 0) {
      buzzer.period(fPeriod);
      buzzer = fDutyCycle;
      xEnabled = true;
    } else {
      buzzer = 0.0;
      xEnabled = false;
    }
  }
}

} // namespace pwm_task
