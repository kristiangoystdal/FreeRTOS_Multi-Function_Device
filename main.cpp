#include "mbed.h"
#include "C12832.h"
#include "LCD.h"
#include "FreeRTOS.h"
#include "task.h"

Serial pc(USBTX,USBRX);

void vTask1(void *pvParameters);


int main()
{
    pc.baud(115200);
    setup_lcd();
    setup_accelerometer();
    setup_temp_sensor();
    xTaskCreate( vTask1, "Task 1", 2*configMINIMAL_STACK_SIZE, NULL, 1, NULL );

    vTaskStartScheduler();

    for( ;; );
    return 0;
}

void vTask1(void *pvParameters){
    while(1) {
        //read X,Y +/-Gs and scale for #display pixels
        write_time(12,12,12);
        write_alarm_enables(true,true);
        write_temperature();
        draw_bubble_level();
        wait(.1); //time delay
    }
}
