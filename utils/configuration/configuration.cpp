
#include "configuration.hpp"
#include "semphr.h"
#include <stdio.h>

namespace configuration {

  static SemaphoreHandle_t xMutexPMON;
  static SemaphoreHandle_t xMutexTALA;
  static SemaphoreHandle_t xMutexBubbleLevel;
  static SemaphoreHandle_t xMutexHitBit;
  static SemaphoreHandle_t xMutexConfigSound;

  static TickType_t xPMON;
  static TickType_t xTALA;
  static bool xBubbleLevelEnabled;
  static bool xHitBitEnabled;
  static bool xConfigSoundEnabled;
  
  void vConfigInitializer() {
    xMutexPMON = xSemaphoreCreateMutex();
    if(xMutexPMON == NULL) {
     printf("Critical error when creating PMON's Mutex!");
    }

    xMutexTALA = xSemaphoreCreateMutex();
    if(xMutexTALA == NULL) {
     printf("Critical error when creating TALA's Mutex!");
    }

    xMutexBubbleLevel = xSemaphoreCreateMutex();
    if(xMutexBubbleLevel == NULL) {
     printf("Critical error when creating Bubble Level's Mutex!");
    }

    xMutexHitBit = xSemaphoreCreateMutex();
    if(xMutexHitBit == NULL) {
     printf("Critical error when creating Hit Bit's Mutex!");
    }

    xMutexConfigSound = xSemaphoreCreateMutex();
    if(xMutexConfigSound == NULL) {
     printf("Critical error when creating Config Sound's Mutex!");
    }

    xPMON = pdMS_TO_TICKS(1000*PMON_DEFAULT_VALUE);
    xTALA = pdMS_TO_TICKS(1000*TALA_DEFAULT_VALUE);
    xBubbleLevelEnabled = true;
    xHitBitEnabled = false;
    xConfigSoundEnabled = false;
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

  bool xConfigGetBubbleLevelEnabled() {
    xSemaphoreTake(xMutexBubbleLevel, portMAX_DELAY);
    bool xEnabled = xBubbleLevelEnabled;
    xSemaphoreGive(xMutexBubbleLevel);
    return xEnabled;
  }

  bool xConfigGetHitBitEnabled() {
    xSemaphoreTake(xMutexHitBit, portMAX_DELAY);
    bool xEnabled = xHitBitEnabled;
    xSemaphoreGive(xMutexHitBit);
    return xEnabled;
  }

  bool xConfigGetConfigSoundEnabled() {
    xSemaphoreTake(xMutexConfigSound, portMAX_DELAY);
    bool xEnabled = xConfigSoundEnabled;
    xSemaphoreGive(xMutexConfigSound);
    return xEnabled;
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

  void vConfigSetBubbleLevelEnabled(bool enabled) {
    xSemaphoreTake(xMutexBubbleLevel, portMAX_DELAY);
    xBubbleLevelEnabled = enabled;
    xSemaphoreGive(xMutexBubbleLevel);
  }

  void vConfigSetHitBitEnabled(bool enabled) {
    xSemaphoreTake(xMutexHitBit, portMAX_DELAY);
    xHitBitEnabled = enabled;
    xSemaphoreGive(xMutexHitBit);
  }

  void vConfigSetConfigSoundEnabled(bool enabled) {
    xSemaphoreTake(xMutexConfigSound, portMAX_DELAY);
    xConfigSoundEnabled = enabled;
    xSemaphoreGive(xMutexConfigSound);
  }

}
