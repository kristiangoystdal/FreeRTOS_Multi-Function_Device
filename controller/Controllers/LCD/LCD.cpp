#include "LCD.h"

C12832 lcd(p5, p7, p6, p8, p11);

void setup_lcd(void){
    lcd.cls();
    lcd.locate(0,0);
}

void write_time(int hours, int minutes, int seconds){
    lcd.locate(0,0);
    lcd.printf("%.02d:%02d:%02d\n", hours, minutes, seconds);
}

void draw_bubble_level(void){
    lcd.line(95,0,95,31,1);
    lcd.circle(113, 16, 3, 1);
}

void write_alarm_enables(bool clock_alarm_enabled, bool temp_alarm_enabled){
    lcd.locate(0, 12);
    lcd.printf("A:");
    if(clock_alarm_enabled){
        lcd.locate(10, 12);
        lcd.printf("C");
    }
    if(temp_alarm_enabled){
        lcd.locate(20,12);
        lcd.printf("T");
    }
}

void write_temperature(){
    lcd.locate(0,22);
    lcd.printf("Temp = 69");
}

