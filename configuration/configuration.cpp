
#include "configuration.hpp"
#include "semphr.h"
#include <stdio.h>

namespace configuration {

  SemaphoreHandle_t xMutexPMON;
  SemaphoreHandle_t xMutexTALA;

  BaseType_t xPMON;
  BaseType_t xTALA;
  
  void vConfigInitializer() {
    xMutexPMON = xSemaphoreCreateMutex();
    if(xMutexPMON == NULL) {
     printf("Critical error when creating PMON's Mutex!");
    }

    xMutexTALA = xSemaphoreCreateMutex();
    if(xMutexTALA == NULL) {
     printf("Critical error when creating TALA's Mutex!");
    }

    xPMON = PMON_DEFAULT_VALUE;
    xTALA = TALA_DEFAULT_VALUE;
  }

  BaseType_t xConfigGetPMON() {
    xSemaphoreTake(xMutexPMON, portMAX_DELAY);
    BaseType_t xPMONValue = xPMON;
    xSemaphoreGive(xMutexPMON);
    return xPMONValue;
  }

  BaseType_t xConfigGetTALA() {
    xSemaphoreTake(xMutexTALA, portMAX_DELAY);
    BaseType_t xTALAValue = xTALA;
    xSemaphoreGive(xMutexTALA);
    return xTALAValue;
  }
  
  void vConfigSetPMON(BaseType_t value) {
    xSemaphoreTake(xMutexPMON, portMAX_DELAY);
    xPMON = value;
    xSemaphoreGive(xMutexPMON);
  }

  void vConfigSetTALA(BaseType_t value) {
    xSemaphoreTake(xMutexTALA, portMAX_DELAY);
    xTALA = value;
    xSemaphoreGive(xMutexTALA);
  }

}