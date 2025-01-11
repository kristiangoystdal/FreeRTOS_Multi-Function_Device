
#include "hit_bit_task.hpp"
#include "FreeRTOS.h"
#include "atomic.hpp"
#include "configuration.hpp"
#include "mbed.h"
#include "task.h"
#include <stdio.h>
#include "global.h"

namespace hit_bit_task {

static DigitalOut led1(LED1);
static DigitalOut led2(LED2);
static DigitalOut led3(LED3);
static DigitalOut led4(LED4);
static InterruptIn pb(p14);

static atomic::Atomic<bool> *xHitBitEnabled;

void vButtonPressed() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(xHitBitHandler, &xHigherPriorityTaskWoken);
}

void setLEDs(BaseType_t xLEDs) {
  led1 = (xLEDs) & 0x01;
  led2 = (xLEDs >> 1) & 0x01;
  led3 = (xLEDs >> 2) & 0x01;
  led4 = (xLEDs >> 3) & 0x01;
}

void vPlayMode() {
  uint32_t ulNotificationValue = 0;
  TickType_t xUpdateTime = pdMS_TO_TICKS(PLAY_MODE_UPDATE_TIME);
  TickType_t xDelay = pdMS_TO_TICKS(JOYSTICK_DELAY_TIME);
  BaseType_t xLEDs = 0xC;
  bool xPlaying = true;
  printf("Playing!");
  while (xPlaying) {
    ulNotificationValue = ulTaskNotifyTake(pdTRUE, xUpdateTime);
    if (ulNotificationValue > 0) {
      printf("Notifity!\n");
      vTaskDelay(xDelay); // Debounce
      int a = pb.read();
      printf("New: %d\n", a);
      xLEDs ^= a;
    } else {
      xLEDs = ((xLEDs & 0x01) << 3) | xLEDs >> 1;
    }
    setLEDs(xLEDs);
    if (xLEDs == 0) {
      xPlaying = false;
    }
  }
}

void vWinMode() {
  BaseType_t xCount = 0;
  TickType_t xUpdateTime = pdMS_TO_TICKS(WIN_MODE_UPDATE_TIME);
  printf("Win!\n");
  while (xCount < 6) {
    setLEDs((xCount % 2 == 0) ? 0xF : 0x0);
    xCount++;
    vTaskDelay(xUpdateTime);
  }
}

bool xGetHitBitEnabled() { return xHitBitEnabled->get(); }

void vSetHitBitEnabled(bool enabled) {
  xHitBitEnabled->set(enabled);
  if (enabled) {
    vTaskResume(xHitBitHandler);
  } else {
    vTaskSuspend(xHitBitHandler);
  }
}

void vHitBitTask(void *pvParameters) {
  printf("Hit Bit Task\n");
  xHitBitEnabled = new atomic::Atomic<bool>(false);
  pb.rise(&vButtonPressed);
  for (;;) {
    vPlayMode();
    vWinMode();
  }
}

} // namespace hit_bit_task
