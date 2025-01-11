
#ifndef __DATE_TIME_H
#define __DATE_TIME_H

#include <time.h>

namespace date_time {

time_t get_time();
tm *get_time_tm();
void get_date(char *s);
void get_clock(char *s);

void set_time_t(time_t t);
void set_dateTime(char *s);

time_t clock_to_time(tm *t);
tm *time_to_clock(time_t t);
void convertTimeToDateClockString(time_t xTime, char *timeString,
                                  size_t bufferSize);
void convertTimeToClockString(time_t xTime, char *timeString,
                              size_t bufferSize);

} // namespace date_time

#endif // __DATE_TIME_H
