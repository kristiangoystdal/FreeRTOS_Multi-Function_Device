
#include "config_sound_task.hpp"
#include "FreeRTOS.h"
#include "atomic.hpp"
#include "configuration.hpp"
#include "mbed.h"
#include "portmacro.h"
#include "task.h"
#include "timers.h"

namespace config_sound_task {

static AnalogIn p1(p19);
static AnalogIn p2(p20);
static PwmOut buzzer(p26);

static float fPeriod = 0.0;
static float fDutyCycle = 0.0;

const float min_output = 0.001;
const float max_output = 0.003;

static TimerHandle_t xTimer;

static atomic::Atomic<bool> *xConfigSoundEnabled;

void vTimerCallback(TimerHandle_t xTimer) {
  fPeriod = min_output + (max_output - min_output) * p1;
  fDutyCycle = p2;
}

bool xGetConfigSoundEnabled() { return xConfigSoundEnabled->get(); }

void vSetConfigSoundEnabled(bool enabled) {
  xConfigSoundEnabled->set(enabled);
  if(enabled) {
    if (xTimerStart(xTimer, 0) != pdPASS) {
      printf("Failed to start timer!\n");
    }
  } else {
    if (xTimerStop(xTimer, 0) != pdPASS) {
      printf("Failed to stop timer!\n");
    }
  }
}

void vConfigSoundTask(void *pvParameters) {
  printf("Config Sound Task\n");
  xConfigSoundEnabled = new atomic::Atomic<bool>(false);
  bool xEnabled = false;
  TickType_t xUpdate = pdMS_TO_TICKS(CONFIG_SOUND_UPDATE_TIME);
  xTimer = xTimerCreate("Config Sound Timer", xUpdate, pdTRUE, (void *)0, vTimerCallback);
  for (;;) {
    TickType_t xTALA =
        xEnabled ? configuration::xConfigGetTALA() : portMAX_DELAY;
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xTALA);
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

} // namespace config_sound_task
