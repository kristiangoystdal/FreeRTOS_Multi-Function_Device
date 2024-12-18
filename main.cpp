#include "C12832.h"
#include "CMD/cmd.h"
#include "FreeRTOS.h"
#include "LCD/LCD.h"
#include "LM75B.h"
#include "RTC.h"
#include "alarm_task.hpp"
#include "lcd_task.hpp"
#include "mbed.h"
#include "queue.h"
#include "task.h"
#include "tasks_macros.h"
#include "temperature_task.hpp"
#include "max_min_task.hpp"
#include "configuration.hpp"

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
void check_tasks(void);
QueueHandle_t createQueue(UBaseType_t uxSize, UBaseType_t uxType);

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
  case 3:
    check_tasks();
    break;
  default:
    break;
  }
  return 0;
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

void check_tasks() {

  QueueHandle_t xQueueMaxMin = createQueue(MAX_MIN_TASK_QUEUE_SIZE, sizeof(max_min_task::MaxMinMessage_t));
  QueueHandle_t xQueueAlarm = createQueue(ALARM_TASK_QUEUE_SIZE, sizeof(alarm_task::AlarmMessage_t));
  QueueHandle_t xQueueLCD = createQueue(LCD_TASK_QUEUE_SIZE, sizeof(lcd_task::LCDMessage_t));
  QueueHandle_t xQueueConsole = createQueue(CONSOLE_TASK_QUEUE_SIZE, 100); // TODO: Think about this

  QueueHandle_t pxReadTemperatureParameters [4] = {xQueueMaxMin, xQueueAlarm, xQueueLCD, xQueueConsole}; 
  QueueHandle_t pxMaxMinParameters [2] = {xQueueMaxMin, xQueueConsole}; 

  configuration::vConfigInitializer();
  max_min_task::vMaxMinInitialize();

  TaskHandle_t xReadTemperatureTaskHandler;
  xTaskCreate(temperature_task::vTemperatureTask, "Task Read Temperature", 2 * configMINIMAL_STACK_SIZE, &pxReadTemperatureParameters, TEMPERATURE_TASK_PRIORITY, &xReadTemperatureTaskHandler);
  xTaskCreate(max_min_task::vMaxMinTask, "Task Max Min", 2 * configMINIMAL_STACK_SIZE, &pxMaxMinParameters, MAX_MIN_TASK_PRIORITY, NULL);

  vTaskStartScheduler();
  while(1);
}

QueueHandle_t createQueue(UBaseType_t uxSize, UBaseType_t uxType) {
  QueueHandle_t xQueue = xQueueCreate(uxSize, uxType);
  if (xQueue == NULL) {
    printf("Failed to create the queue\n");
  }
  return xQueue;
}
