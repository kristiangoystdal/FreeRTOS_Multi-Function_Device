
#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "FreeRTOS.h"

namespace configuration {

  #define PMON_DEFAULT_VALUE 3
  #define TALA_DEFAULT_VALUE 5

  void vConfigInitializer();

  TickType_t xConfigGetPMON();
  TickType_t xConfigGetTALA();
  bool xConfigGetBubbleLevelEnabled();
  bool xConfigGetHitBitEnabled();
  bool xConfigGetConfigSoundEnabled();
  
  void vConfigSetPMON(int seconds);
  void vConfigSetTALA(int seconds);
  void vConfigSetBubbleLevelEnabled(bool enabled);
  void vConfigSetHitBitEnabled(bool enabled);
  void vConfigSetConfigSoundEnabled(bool enabled);

}

#endif
