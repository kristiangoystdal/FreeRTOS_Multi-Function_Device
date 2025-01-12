
#include "rgb.hpp"
#include "mbed.h"

namespace rgb {

  static PwmOut r (p23);
  static PwmOut g (p24);
  static PwmOut b (p25);

  void set_rgb(float red, float green, float blue) {
    r = 1.0 - red;
    g = 1.0 - green;
    b = 1.0 - blue;
  }

  void set_period() {
    r.period(0.001);
  }

}