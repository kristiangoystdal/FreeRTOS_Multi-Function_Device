#include "LCD.h"
#include "LM75B.h"

static C12832 lcd(p5, p7, p6, p8, p11);

void setup_lcd(void) {
  lcd.cls();
  lcd.locate(0, 0);
  lcd.locate(0, 12);
  lcd.printf("A:");
  lcd.locate(0, 22);
  lcd.printf("Temp =");
  printf("LCD OK\n");
}

void write_time(int hours, int minutes, int seconds) {
  lcd.locate(0, 0);
  lcd.printf("%.02d:%02d:%02d\n", hours, minutes, seconds);
}

void draw_bubble_level(int x, int y) {
  lcd.fillrect(95, 0, 127, 31, 0);
  if (x > -16) {
    lcd.fillcircle(x + 111, y + 15, 3, 1);
  }
  lcd.fillrect(93, 0, 95, 31, 1);
}

void write_clock_alarm(bool clock_alarm_enabled) {
  lcd.locate(10, 12);
  lcd.printf(clock_alarm_enabled ? "C" : " ");
}

void write_temp_alarm(bool temp_alarm_enabled) {
  lcd.locate(20, 12);
  lcd.printf(temp_alarm_enabled ? "T" : " ");
}

void write_temperature(float temperature) {
  lcd.locate(0, 22);
  lcd.printf("Temp = %.1f", temperature);
}
