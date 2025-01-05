
#ifndef __CLOCK_H
#define __CLOCK_H

#include <time.h>

namespace clock {

  time_t get_time();
  void get_date(char* s);
  void get_clock(char* s);

  void set_time(time_t t);
  void set_date(char* s);
  void set_clock(char* s);

}

#endif // __CLOCK_H