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
#include "clock.hpp"
#include "pwm_task.hpp"

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
void check_tasks(void);

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
  time_t seconds = date_time::get_time();
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

QueueHandle_t xCreateQueue(UBaseType_t uxSize, UBaseType_t uxType) {
  QueueHandle_t xQueue = xQueueCreate(uxSize, uxType);
  if (xQueue == NULL) {
    printf("Failed to create the queue\n");
  }
  return xQueue;
}

void vCreateTask(TaskFunction_t pxTaskCode, const char * const pcName, const uint16_t usStackDepth,
							    void * const pvParameters, UBaseType_t uxPriority, TaskHandle_t * const pxCreatedTask) {
  BaseType_t xReturn = xTaskCreate(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask);
  if(xReturn == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY) {
    printf("Failed to create the task %s\n", pcName);
  }
}

void check_tasks() {

  TaskHandle_t xPWMTaskHandler;
  vCreateTask(pwm_task::vPWMTask, "Task PWM", 2 * configMINIMAL_STACK_SIZE, NULL, MAX_MIN_TASK_PRIORITY, &xPWMTaskHandler);

  QueueHandle_t xQueueMaxMin = xCreateQueue(MAX_MIN_TASK_QUEUE_SIZE, sizeof(max_min_task::MaxMinMessage_t));
  QueueHandle_t xQueueAlarm = xCreateQueue(ALARM_TASK_QUEUE_SIZE, sizeof(alarm_task::AlarmMessage_t));
  QueueHandle_t xQueueLCD = xCreateQueue(LCD_TASK_QUEUE_SIZE, sizeof(lcd_task::LCDMessage_t));
  QueueHandle_t xQueueConsole = xCreateQueue(CONSOLE_TASK_QUEUE_SIZE, 100); // TODO: Think about this

  QueueHandle_t pxTemperatureParameters [4] = {xQueueMaxMin, xQueueAlarm, xQueueLCD, xQueueConsole};
  QueueHandle_t pxAlarmParameters [4] = {xQueueAlarm, xQueueConsole, xQueueLCD, xPWMTaskHandler};
  QueueHandle_t pxMaxMinParameters [3] = {xQueueMaxMin, xQueueConsole, xQueueLCD};
  QueueHandle_t pxLCDParameters [4] = {xQueueLCD};

  configuration::vConfigInitializer();
  max_min_task::vMaxMinInitialize();

  TaskHandle_t xTemperatureTaskHandler;
  vCreateTask(temperature_task::vTemperatureTask, "Task Temperature", 2 * configMINIMAL_STACK_SIZE, &pxTemperatureParameters, TEMPERATURE_TASK_PRIORITY, &xTemperatureTaskHandler);
  vCreateTask(alarm_task::vAlarmTask, "Task Alarm", 2 * configMINIMAL_STACK_SIZE, &pxAlarmParameters , ALARM_TASK_PRIORITY, NULL);
  vCreateTask(max_min_task::vMaxMinTask, "Task Max Min", 2 * configMINIMAL_STACK_SIZE, &pxMaxMinParameters, MAX_MIN_TASK_PRIORITY, NULL);
  vCreateTask(lcd_task::vLCDTask, "Task LCD", 2 * configMINIMAL_STACK_SIZE, &pxLCDParameters, LCD_TASK_PRIORITY, NULL);
  vTaskStartScheduler();
  while(1);
}
