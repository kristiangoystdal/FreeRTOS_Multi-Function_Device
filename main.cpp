#include "C12832.h"
#include "CMD/cmd.h"
#include "FreeRTOS.h"
#include "LCD/LCD.h"
#include "LM75B.h"
#include "RTC.h"
#include "mbed.h"
#include "queue.h"
#include "task.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
Serial pc(USBTX, USBRX);

LM75B sensor(p28, p27);

extern void monitor(void);

QueueHandle_t xQueue;

void check_temperature(void);
void check_rtc(void);
void check_lcd(void);
void check_cmd(void);

int main() {
  int testNumber = 2; // TODO: Change this value for do the other tests
  switch (testNumber) {
  case 0:
    check_temperature();
    break;
  case 1:
    check_rtc();
    break;
  case 2:
    check_cmd();
    break;
  default:
    break;
  }
}

void vTask2(void *pvParameters) {
  int32_t lReceivedValue;
  BaseType_t xStatus;

      while (1)
        ;

    } else {
      error("Device not detected!\n");
    }
    led2 = !led2;
  }
}

int main(void) {
  /* Perform any hardware setup necessary. */
  //    prvSetupHardware();

  pc.baud(115200);

  //    printf("Hello from mbed -- FreeRTOS / cmd\n");

  /* --- APPLICATION TASKS CAN BE CREATED HERE --- */

  xQueue = xQueueCreate(3, sizeof(int32_t));

  xTaskCreate(vTask1, "Task 1", 2 * configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(vTask2, "Task 2", 2 * configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  vTaskStartScheduler();

  while (1) {
    tm t = RTC::getDefaultTM();
    // printf("Current time: %d\n", t.tm_sec);
    wait(1);
  };
}
