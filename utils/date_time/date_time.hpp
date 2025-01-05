
#ifndef __DATE_TIME_H
#define __DATE_TIME_H

#include <time.h>

namespace date_time {

  time_t get_time();
  void get_date(char* s);
  void get_clock(char* s);

  void set_time(time_t t);
  void set_date(char* s);
  void set_clock(char* s);

}

#endif // __DATE_TIME_H
