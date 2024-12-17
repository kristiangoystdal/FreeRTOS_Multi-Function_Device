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
  pc.baud(115200);
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

void check_temperature() {
  while (1) {
    // Try to open the LM75B
    if (sensor.open()) {
      printf("Device detected!\n");

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

void displayFunction(void) {
  time_t seconds = time(NULL);
  printf("%s", ctime(&seconds));
}

void alarmFunction(void) { error("Not most useful alarm function"); }

void check_rtc() {
  set_time(1256729737); // Set time to Wed, 28 Oct 2009 11:35:37

  tm t = RTC::getDefaultTM();
  t.tm_sec = 5;
  t.tm_min = 36;

  RTC::alarm(&alarmFunction, t);
  RTC::attach(&displayFunction, RTC::Second);
  RTC::attach(&ledFunction, RTC::Minute);

  while (1)
    ;
}

void check_cmd() {

  init_TaskScheduler(&xQueue);

  xTaskCreate(vTask1, "Task 1", 2 * configMINIMAL_STACK_SIZE, xQueue, 1, NULL);
  xTaskCreate(vTask2, "Task 2", 2 * configMINIMAL_STACK_SIZE, xQueue, 2, NULL);

  vTaskStartScheduler();

  while (1) {
    tm t = RTC::getDefaultTM();
    printf("Current time: %d\n", t.tm_sec);
    wait(1);
  };
}
