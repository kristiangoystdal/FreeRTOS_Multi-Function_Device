
#include "date_time.hpp"

namespace date_time {

  time_t get_time() {
    return time(NULL);
  }

  tm* get_time_tm() {
    time_t t = time(NULL);
    return localtime(&t);
  } 

  void get_date(char* s) {
    time_t t = time(NULL);
    tm tm_;
    localtime_r(&t, &tm_);
    strftime(&s[0], sizeof(s), "%d/%m/%Y %H:%M:%S\n", &tm_);
  }

  void get_clock(char* s) {
    time_t t = time(NULL);
    tm tm_;
    localtime_r(&t, &tm_);
    strftime(&s[0], sizeof(s), "%H:%M:%S\n", &tm_);
  }

  // TODO: Set Time
  void set_time(time_t t) {
  }

  void set_date(char* s) {
  }

  void set_clock(char* s) {
  }

  time_t clock_to_time(tm* t) {
    return mktime(t);
  }

  tm* time_to_clock(time_t t) {
    return localtime(&t);
  }
  
} // namespace clock
