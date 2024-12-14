#include "mbed.h"
#include "C12832.h"
#include "LCD.h"

Serial pc(USBTX,USBRX);


int main()
{
    pc.baud(115200);
    setup_lcd();
    check_accelerometer();

    while(1) {
        //read X,Y +/-Gs and scale for #display pixels
        write_time(12,12,12);
        write_alarm_enables(true,true);
        write_temperature();
        draw_bubble_level();
        wait(.1); //time delay
    }
}