
#include "config_sound_task.hpp"
#include "FreeRTOS.h"
#include "atomic.hpp"
#include "global.h"
#include "mbed.h"
#include "portmacro.h"
#include "task.h"
#include "timers.h"
#include "rgb_task.hpp"

namespace config_sound_task {

static AnalogIn p1(p19);
static AnalogIn p2(p20);
static PwmOut buzzer(p26);

const float min_output = 0.001;
const float max_output = 0.003;

static float fPeriod = 0.0;
static float fDutyCycle = 0.0;

static TimerHandle_t xTimer;

static atomic::Atomic<bool> *xConfigSoundEnabled;
static atomic::Atomic<TickType_t> *xTALA;

void vTimerCallback(TimerHandle_t xTimer) {
  ConfigSoundMessage_t xMessage;
  xMessage.xAction = Stop;
  if (xQueueSend(xQueueConfigSound, &xMessage, 0) == errQUEUE_FULL) {
    printf("ERROR: Queue full: Timer -> Config Sound");
  }
}

bool xGetConfigSoundEnabled() { return xConfigSoundEnabled->get(); }

void vSetConfigSoundEnabled(bool enabled) { xConfigSoundEnabled->set(enabled); }

TickType_t xConfigGetTALA() { return xTALA->get(); }

void vConfigSetTALA(int seconds) { xTALA->set(pdMS_TO_TICKS(1000 * seconds)); }

void vConfigSoundTask(void *pvParameters) {
  printf("Config Sound Task\n");
  xConfigSoundEnabled = new atomic::Atomic<bool>(false);
  bool xEnabled = false;
  TickType_t xUpdate =  pdMS_TO_TICKS(1000 * TALA_DEFAULT_VALUE);
  xTimer = xTimerCreate("Config Sound Timer", xUpdate, pdTRUE, (void *)0,
                        vTimerCallback);
  ConfigSoundMessage_t xMessage;
  xTALA =
      new atomic::Atomic<TickType_t>(pdMS_TO_TICKS(1000 * TALA_DEFAULT_VALUE));
  for (;;) {
    TickType_t xTicks = xEnabled ?  pdMS_TO_TICKS(CONFIG_SOUND_UPDATE_TIME) : portMAX_DELAY;
    if (xQueueReceive(xQueueConfigSound, &xMessage, xTicks) == pdFALSE) {
      if (xGetConfigSoundEnabled()) {
        fPeriod = min_output + (max_output - min_output) * p1;
        fDutyCycle = p2;
        buzzer.period(fPeriod);
        buzzer = fDutyCycle;
      }
      continue;
    }
    switch (xMessage.xAction) {
    case Alarm:
      if (!xEnabled) {
        buzzer.period(fPeriod);
        buzzer = fDutyCycle;
      }
      xUpdate = xConfigGetTALA();
      if (xTimerChangePeriod(xTimer, xUpdate, 0) != pdPASS) {
        printf("Failed to change timer period!\n");
      }
      if (xTimerStart(xTimer, 0) != pdPASS) {
        printf("Failed to start timer!\n");
      }
      xEnabled = true;
      break;
    case Stop:
      buzzer = 0.0;
      xEnabled = false;
      rgb_task::RGBMessage_t xRGBMessage;
      xRGBMessage.xAction = rgb_task::Update;
      if (xQueueSend(xQueueRGB, &xRGBMessage, 0) == errQUEUE_FULL) {
        printf("ERROR: Queue full: Config Sound -> RGB");
      }
      break;
    default:
      break;
    }
  }
}

} // namespace config_sound_task
