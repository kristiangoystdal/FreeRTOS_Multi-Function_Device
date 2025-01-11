
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
  struct tm tm_ = {0}; // Initialize tm structure to zero

  // Print the input string for debugging
  printf("Input string: %s\n", s);

  // Parse the date string into the tm structure
  if (sscanf(s, "%2d/%2d/%4d %2d:%2d:%2d", &tm_.tm_mday, &tm_.tm_mon,
             &tm_.tm_year, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec) != 6) {
    printf("Error while parsing date\n");
    return;
  }

  // Adjust tm structure fields for mktime compatibility
  tm_.tm_year -= 1900; // Convert year to "years since 1900"
  tm_.tm_mon -= 1;     // Convert month to zero-based (0 = January)

  // Debugging: Print the parsed tm structure
  printf("Parsed tm structure:\n");
  printf("  Day: %d\n", tm_.tm_mday);
  printf("  Month: %d\n",
         tm_.tm_mon + 1); // Convert back to 1-based for display
  printf("  Year: %d\n",
         tm_.tm_year + 1900); // Convert back to full year for display
  printf("  Hour: %d\n", tm_.tm_hour);
  printf("  Minute: %d\n", tm_.tm_min);
  printf("  Second: %d\n", tm_.tm_sec);

  // Convert tm to time_t using mktime
  time_t t = clock_to_time(&tm_);
  if (t == -1) {
    printf("Error: mktime failed\n");
    return;
  }

  // Debugging: Print the resulting time_t value
  printf("time_t value: %ld\n", t);

  // Set the system time using set_time
  set_time(t);
  printf("Date successfully set.\n");
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
