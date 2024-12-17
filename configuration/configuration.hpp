
#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "FreeRTOS.h"

namespace configuration {

  #define PMON_DEFAULT_VALUE 3
  #define TALA_DEFAULT_VALUE 5

  void vConfigInitializer();

  BaseType_t xConfigGetPMON();
  BaseType_t xConfigGetTALA();
  
  void vConfigSetPMON(BaseType_t value);
  void vConfigSetTALA(BaseType_t value);

}

#endif
