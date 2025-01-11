
#include "date_time.hpp"
#include "mbed.h"

namespace date_time {

time_t get_time() { return time(NULL); }

tm *get_time_tm() {
  time_t t = time(NULL);
  return localtime(&t);
}

void get_date(char *s) {
  time_t t = time(NULL);
  tm tm_;
  localtime_r(&t, &tm_);
  strftime(&s[0], sizeof(s), "%d/%m/%Y %H:%M:%S\n", &tm_);
}

void get_clock(char *s) {
  time_t t = time(NULL);
  struct tm tm_;
  localtime_r(&t, &tm_);

  // Ensure a safe, proper size is used for the buffer
  strftime(s, 9, "%H:%M:%S", &tm_); // Format the time as "hh:mm:ss"
}

void set_time_t(time_t t) { set_time(t); }

void set_date(char *s) {
  tm tm_;
  printf("%s", s);
  if (sscanf(s, "%2d/%2d/%4d %2d:%2d:%2d", &tm_.tm_mday, &tm_.tm_mon,
             &tm_.tm_year, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec) != 6) {
    printf("Error while parsing date");
    return;
  }
  set_time(clock_to_time(&tm_));
}

void set_clock(char *s) {
  tm tm_;
  if (sscanf(s, "%2d:%2d:%2d", &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec) != 3) {
    printf("Error while parsing clock");
    return;
  }
  set_time(clock_to_time(&tm_));
}

time_t clock_to_time(tm *t) { return mktime(t); }

tm *time_to_clock(time_t t) { return localtime(&t); }

void convertTimeToString(time_t xTime, char *timeString, size_t bufferSize) {
  struct tm *timeInfo = localtime(&xTime);

  strftime(timeString, bufferSize, "%d/%m/%Y %H:%M:%S", timeInfo);
}

} // namespace date_time
