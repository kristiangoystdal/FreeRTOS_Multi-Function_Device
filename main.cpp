#include "mbed.h"
#include "RTC.h"

DigitalOut led(LED1);

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

int main()
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
