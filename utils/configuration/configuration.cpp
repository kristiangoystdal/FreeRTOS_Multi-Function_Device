
#include "configuration.hpp"
#include "semphr.h"
#include <stdio.h>

namespace configuration {

  static SemaphoreHandle_t xMutexPMON;
  static SemaphoreHandle_t xMutexTALA;

  static TickType_t xPMON;
  static TickType_t xTALA;
  
  void vConfigInitializer() {
    xMutexPMON = xSemaphoreCreateMutex();
    if(xMutexPMON == NULL) {
     printf("Critical error when creating PMON's Mutex!");
    }

    xMutexTALA = xSemaphoreCreateMutex();
    if(xMutexTALA == NULL) {
     printf("Critical error when creating TALA's Mutex!");
    }

    xPMON = pdMS_TO_TICKS(1000*PMON_DEFAULT_VALUE);
    xTALA = pdMS_TO_TICKS(1000*TALA_DEFAULT_VALUE);
  }

  TickType_t xConfigGetPMON() {
    xSemaphoreTake(xMutexPMON, portMAX_DELAY);
    TickType_t xPMONValue = xPMON;
    xSemaphoreGive(xMutexPMON);
    return xPMONValue;
  }

  TickType_t xConfigGetTALA() {
    xSemaphoreTake(xMutexTALA, portMAX_DELAY);
    TickType_t xTALAValue = xTALA;
    xSemaphoreGive(xMutexTALA);
    return xTALAValue;
  }
  
  void vConfigSetPMON(int seconds) {
    TickType_t ticks = pdMS_TO_TICKS(1000*seconds);
    xSemaphoreTake(xMutexPMON, portMAX_DELAY);
    xPMON = ticks;
    xSemaphoreGive(xMutexPMON);
  }

  void vConfigSetTALA(int seconds) {
    TickType_t ticks = pdMS_TO_TICKS(1000*seconds);
    xSemaphoreTake(xMutexTALA, portMAX_DELAY);
    xTALA = ticks;
    xSemaphoreGive(xMutexTALA);
  }

}
