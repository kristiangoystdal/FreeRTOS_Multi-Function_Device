
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
  struct tm tm_;
  localtime_r(&t, &tm_);

  strftime(s, 20, "%d/%m/%Y %H:%M:%S", &tm_);
}

void get_clock(char *s) {
  time_t t = time(NULL);
  struct tm tm_;
  localtime_r(&t, &tm_);

  strftime(s, 9, "%H:%M:%S", &tm_); // Format the time as "hh:mm:ss"
}

void set_time_t(time_t t) { set_time(t); }

void set_dateTime(char *s) {
  struct tm tm_ = {0};

  if (sscanf(s, "%2d/%2d/%4d %2d:%2d:%2d", &tm_.tm_mday, &tm_.tm_mon,
             &tm_.tm_year, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec) != 6) {
    printf("Error while parsing date\n");
    return;
  }

  tm_.tm_year -= 1900; // Convert year to "years since 1900"
  tm_.tm_mon -= 1;     // Convert month to zero-based (0 = January)

  time_t t = clock_to_time(&tm_);
  if (t == -1) {
    printf("Error: mktime failed\n");
    return;
  }

  set_time(t);
}

time_t clock_to_time(tm *t) { return mktime(t); }

tm *time_to_clock(time_t t) { return localtime(&t); }

void convertTimeToDateClockString(time_t xTime, char *timeString,
                                  size_t bufferSize) {
  struct tm *timeInfo = localtime(&xTime);

  strftime(timeString, bufferSize, "%d/%m/%Y %H:%M:%S", timeInfo);
}

void convertTimeToClockString(time_t xTime, char *timeString,
                              size_t bufferSize) {
  struct tm *timeInfo = localtime(&xTime);

  strftime(timeString, bufferSize, "%H:%M:%S", timeInfo);
}

time_t integer_to_time_t(int H, int M, int S) {
  time_t now = time(NULL);
  struct tm timeinfo = *localtime(&now);
  timeinfo.tm_hour = H;
  timeinfo.tm_min = M;
  timeinfo.tm_sec = S;
  return mktime(&timeinfo);
}

} // namespace date_time
