
#include "configuration.hpp"
#include "semphr.h"
#include <stdio.h>

namespace configuration {

  SemaphoreHandle_t xMutexPMON;
  SemaphoreHandle_t xMutexTALA;

  TickType_t xPMON;
  TickType_t xTALA;
  
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
  
  void vConfigSetPMON(TickType_t value) {
    xSemaphoreTake(xMutexPMON, portMAX_DELAY);
    xPMON = value;
    xSemaphoreGive(xMutexPMON);
  }

  void vConfigSetTALA(TickType_t value) {
    xSemaphoreTake(xMutexTALA, portMAX_DELAY);
    xTALA = value;
    xSemaphoreGive(xMutexTALA);
  }

}