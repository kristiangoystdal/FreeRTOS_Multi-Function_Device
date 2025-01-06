#ifndef __LCD_H
#define __LCD_H

#include "C12832.h"



void setup_lcd(void);

void setup_accelerometer(void);

void setup_temp_sensor(void);

void write_time(int hours, int minutes, int seconds);

void draw_bubble_level(void);

void write_clock_alarm(bool clock_alarm_enabled);

void write_temp_alarm(bool temp_alarm_enabled);

void write_temperature(int temperature);

#endif
