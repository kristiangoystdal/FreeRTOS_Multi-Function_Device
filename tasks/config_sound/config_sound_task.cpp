
#include "config_sound_task.hpp"
#include "FreeRTOS.h"
#include "atomic.hpp"
#include "configuration.hpp"
#include "mbed.h"
#include "portmacro.h"
#include "task.h"
#include "timers.h"
#include "global.h"

namespace config_sound_task {

static AnalogIn p1(p19);
static AnalogIn p2(p20);
static PwmOut buzzer(p26);

const float min_output = 0.001;
const float max_output = 0.003;

static TimerHandle_t xTimer;

static atomic::Atomic<bool> *xConfigSoundEnabled;

void vTimerCallback(TimerHandle_t xTimer) {
  ConfigSoundMessage_t xMessage;
  xMessage.xAction = Update;
  xMessage.xData.xPeriod = min_output + (max_output - min_output) * p1;
  xMessage.xData.xDutyCycle = p2;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xQueueConfigSound, &xMessage, &xHigherPriorityTaskWoken);
}

bool xGetConfigSoundEnabled() { return xConfigSoundEnabled->get(); }

void vSetConfigSoundEnabled(bool enabled) { xConfigSoundEnabled->set(enabled); }

void vConfigSoundTask(void *pvParameters) {
  printf("Config Sound Task\n");
  xConfigSoundEnabled = new atomic::Atomic<bool>(false);
  bool xEnabled = false;
  TickType_t xUpdate = pdMS_TO_TICKS(CONFIG_SOUND_UPDATE_TIME);
  xTimer = xTimerCreate("Config Sound Timer", xUpdate, pdTRUE, (void *)0, vTimerCallback);
  ConfigSoundMessage_t xMessage;
  BaseType_t xStatus;
  for (;;) {
    TickType_t xTALA =
        xEnabled ? configuration::xConfigGetTALA() : portMAX_DELAY;
    xStatus = xQueueReceive(xQueueConfigSound, &xMessage, xTALA);
    if(xStatus == pdFALSE) {
      if (xTimerStop(xTimer, 0) != pdPASS) {
        printf("Failed to stop timer!\n");
      }
      xEnabled = false;
      continue;
    }
    switch (xMessage.xAction) {
    case Alarm:
      if(!xEnabled) {
        buzzer.period(min_output + (max_output - min_output) * p1);
        buzzer = p2;
      }
      if(xGetConfigSoundEnabled()) {
        if (xTimerStart(xTimer, 0) != pdPASS) {
          printf("Failed to start timer!\n");
        }
      }
      xEnabled = true;
      break;
    case Update:
      buzzer.period(xMessage.xData.xPeriod);
      buzzer = xMessage.xData.xDutyCycle;
      break;
    default:
      break;
    }
  }
}

} // namespace config_sound_task
