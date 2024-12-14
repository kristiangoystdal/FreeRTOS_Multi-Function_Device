#include "C12832.h"
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

DigitalOut led(LED1);

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
  return 0;
}

void check_temperature() {
  pc.baud(115200);
  while (1) {
    // Try to open the LM75B
    if (sensor.open()) {
      printf("Device detected!\n");

      while (1)
        ;

    } else {
      error("Device not detected!\n");
    }
  }
}

void ledFunction(void) {
  led = 1;
  RTC::detach(RTC::Second);
}

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
  pc.baud(115200);

  xQueue = xQueueCreate(3, sizeof(int32_t));

  while (1) {
    tm t = RTC::getDefaultTM();
    printf("Current time: %d\n", t.tm_sec);
    wait(1);
  };
}
