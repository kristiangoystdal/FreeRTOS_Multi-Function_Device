
#include "configuration.hpp"
#include "atomic.hpp"
#include <stdio.h>

namespace configuration {

  static atomic::Atomic<TickType_t> xPMON;
  static atomic::Atomic<TickType_t> xTALA;
  
  void vConfigInitializer() {
    xPMON = new atomic::Atomic<TickType_t>(pdMS_TO_TICKS(1000*PMON_DEFAULT_VALUE));
    xTALA = new atomic::Atomic<TickType_t>(pdMS_TO_TICKS(1000*TALA_DEFAULT_VALUE));
  }

  TickType_t xConfigGetPMON() {
    return xPMON.get();
  }

  TickType_t xConfigGetTALA() {
    return xTALA.get();
  }
  
  void vConfigSetPMON(int seconds) {
    xPMON.set(pdMS_TO_TICKS(1000*seconds));
  }

  void vConfigSetTALA(int seconds) {
    xTALA.set(pdMS_TO_TICKS(1000*seconds));
  }

}
