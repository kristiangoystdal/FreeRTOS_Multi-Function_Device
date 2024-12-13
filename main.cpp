#include "mbed.h"
#include "RTC.h"
#include "LM75B.h"
#include "C12832.h"

C12832 lcd(p5, p7, p6, p8, p11);

LM75B sensor(p28,p27);
Serial pc(USBTX,USBRX);

DigitalOut led(LED1);

void check_temperature(void);
void check_rtc(void);
void check_lcd(void);

int main ()
{
    int testNumber = 0; // TODO: Change this value for do the other tests
    switch (testNumber) {
    case 0:
        check_temperature();
        break;
    case 1:
        check_rtc();
        break;
    }
    return 0;
}

void check_temperature() {
    pc.baud(115200);
    while (1) {
        //Try to open the LM75B
        if (sensor.open()) {
            printf("Device detected!\n");

            while (1) {
                lcd.cls();
                lcd.locate(0,3);
                lcd.printf("Temp = %.3f\n", sensor.temp());
                wait(1.0);
            }

        } else {
            error("Device not detected!\n");
        }
    }
}

void ledFunction( void )
{
    led = 1;
    RTC::detach(RTC::Second);
}

void displayFunction( void )
{
    time_t seconds = time(NULL);
    printf("%s", ctime(&seconds));
}

void alarmFunction( void )
{
    error("Not most useful alarm function");
}

void check_rtc()
{
    set_time(1256729737); // Set time to Wed, 28 Oct 2009 11:35:37

    tm t = RTC::getDefaultTM();
    t.tm_sec = 5;
    t.tm_min = 36;

    RTC::alarm(&alarmFunction, t);
    RTC::attach(&displayFunction, RTC::Second);
    RTC::attach(&ledFunction, RTC::Minute);

    while(1);
}
