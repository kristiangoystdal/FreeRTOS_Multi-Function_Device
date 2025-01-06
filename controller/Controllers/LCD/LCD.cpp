#include "LCD.h"
#include "MMA7660.h"
#include "LM75B.h"

static MMA7660 MMA(p28, p27); //I2C Accelerometer
static C12832 lcd(p5, p7, p6, p8, p11);
static LM75B sensor(p28,p27);

static int bubble_x=0, bubble_y=0;

void setup_lcd(void){
    lcd.cls();
    lcd.locate(0,0);
    lcd.locate(0, 12);
    lcd.printf("A:");
    lcd.locate(0,22);
    lcd.printf("Temp =");
    printf("LCD OK");
}

void setup_accelerometer(void){
    if(MMA.testConnection()){
        printf("Accelerometer OK \n");
    } else {
        printf("Accelerometer NOT OK");
    }
}

void setup_temp_sensor(void){
    if(sensor.open()){
        printf("Temperature sensor OK");
    }else{
        printf("Temperature sensor NOT OK");
    }
}

void write_time(int hours, int minutes, int seconds){
    lcd.locate(0,0);
    lcd.printf("%.02d:%02d:%02d\n", hours, minutes, seconds);
}

void draw_bubble_level(void){
        lcd.fillrect(95, 0, 127, 31, 0);
        bubble_x = (bubble_x + MMA.x() * 32.0)/2.0;
        bubble_y = (bubble_y -(MMA.y() * 16.0))/2.0;
        if(bubble_x>-16){
            lcd.fillcircle(bubble_x+111, bubble_y+15, 3, 1);
        }
        lcd.fillrect(93,0,95,31,1);
}

void write_clock_alarm(bool clock_alarm_enabled){
    lcd.locate(10, 12);
    lcd.printf(clock_alarm_enabled ? "C" : " ");
}

void write_temp_alarm(bool temp_alarm_enabled){
    lcd.locate(20,12);
    lcd.printf(temp_alarm_enabled ? "T" : " ");
}

void write_temperature(int temperature){
    lcd.locate(0,22);
    lcd.printf("Temp = %d", temperature);
}

