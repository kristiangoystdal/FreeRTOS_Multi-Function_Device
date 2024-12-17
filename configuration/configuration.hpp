
#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "FreeRTOS.h"

namespace configuration {

  #define PMON_DEFAULT_VALUE 3
  #define TALA_DEFAULT_VALUE 5

  void vConfigInitializer();

  TickType_t xConfigGetPMON();
  TickType_t xConfigGetTALA();
  
  void vConfigSetPMON(TickType_t value);
  void vConfigSetTALA(TickType_t value);

}

#endif
