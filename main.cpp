#include "C12832.h"
#include "CMD/cmd.h"
#include "FreeRTOS.h"
#include "LCD/LCD.h"
#include "LM75B.h"
#include "RTC.h"
#include "alarm_task.hpp"
#include "bubble_level_task.hpp"
#include "comando.hpp"
#include "command_task.hpp"
#include "configuration.hpp"
#include "hit_bit_task.hpp"
#include "lcd_task.hpp"
#include "max_min_task.hpp"
#include "mbed.h"
#include "pwm_task.hpp"
#include "queue.h"
#include "task.h"
#include "tasks_macros.h"
#include "temperature_task.hpp"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
Serial pc(USBTX, USBRX);

// LM75B sensor(p28, p27);
I2C i2c(p28, p27);

void check_tasks(void);

QueueHandle_t createQueue(UBaseType_t uxSize, UBaseType_t uxType);

int main() {
  pc.baud(115200);
  check_tasks();
  return 0;
}

void scanI2CDevices() {
  printf("Scanning I2C bus...\n");
  for (int address = 0; address < 128; address++) {
    if (i2c.write(address << 1, NULL, 0) == 0) {
      printf("Device found at address 0x%X\n", address);
    }
  }
}

void vCreateTask(TaskFunction_t pxTaskCode, const char *const pcName,
                 const uint16_t usStackDepth, void *const pvParameters,
                 UBaseType_t uxPriority, TaskHandle_t *const pxCreatedTask) {
  BaseType_t xReturn = xTaskCreate(pxTaskCode, pcName, usStackDepth,
                                   pvParameters, uxPriority, pxCreatedTask);
  if (xReturn == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY) {
    printf("Failed to create the task %s\n", pcName);
  }
}

QueueHandle_t xCreateQueue(UBaseType_t uxSize, UBaseType_t uxType) {
  QueueHandle_t xQueue = xQueueCreate(uxSize, uxType);
  if (xQueue == NULL) {
    printf("Failed to create the queue\n");
  }
  return xQueue;
}

void check_tasks() {

  TaskHandle_t xPWMTaskHandler;
  vCreateTask(pwm_task::vPWMTask, "Task PWM", 2 * configMINIMAL_STACK_SIZE,
              NULL, MAX_MIN_TASK_PRIORITY, &xPWMTaskHandler);

  QueueHandle_t xQueueMaxMin = xCreateQueue(
      MAX_MIN_TASK_QUEUE_SIZE, sizeof(max_min_task::MaxMinMessage_t));
  QueueHandle_t xQueueAlarm =
      xCreateQueue(ALARM_TASK_QUEUE_SIZE, sizeof(alarm_task::AlarmMessage_t));
  QueueHandle_t xQueueLCD =
      xCreateQueue(LCD_TASK_QUEUE_SIZE, sizeof(lcd_task::LCDMessage_t));
  QueueHandle_t xQueueConsole =
      xCreateQueue(CONSOLE_TASK_QUEUE_SIZE, 100); // TODO: Think about this

  QueueHandle_t pxTemperatureParameters[4] = {xQueueMaxMin, xQueueAlarm,
                                              xQueueLCD, xQueueConsole};
  QueueHandle_t pxAlarmParameters[4] = {xQueueAlarm, xQueueConsole, xQueueLCD,
                                        xPWMTaskHandler};
  QueueHandle_t pxMaxMinParameters[3] = {xQueueMaxMin, xQueueConsole,
                                         xQueueLCD};
  QueueHandle_t pxLCDParameters[1] = {xQueueLCD};

  QueueHandle_t pxCmdParameters[4] = {xQueueConsole, xQueueMaxMin, xQueueAlarm,
                                      xQueueLCD};

  configuration::vConfigInitializer();
  comando::vCommandInitialize(&pxCmdParameters);
  scanI2CDevices();

  printf("Init complete...");

  // xTaskCreate(vMonitorTask, "Monitor", 2 * configMINIMAL_STACK_SIZE, NULL, 1,
  //             NULL);

  TaskHandle_t xTemperatureTaskHandler;
  vCreateTask(temperature_task::vTemperatureTask, "Task Temperature",
              2 * configMINIMAL_STACK_SIZE, &pxTemperatureParameters,
              TEMPERATURE_TASK_PRIORITY, &xTemperatureTaskHandler);
  // vCreateTask(alarm_task::vAlarmTask, "Task Alarm",
  //             2 * configMINIMAL_STACK_SIZE, &pxAlarmParameters,
  //             ALARM_TASK_PRIORITY, NULL);
  // vCreateTask(max_min_task::vMaxMinTask, "Task Max Min",
  //             2 * configMINIMAL_STACK_SIZE, &pxMaxMinParameters,
  //             MAX_MIN_TASK_PRIORITY, NULL);
  // vCreateTask(lcd_task::vLCDTask, "Task Hit Bit", 2 *
  // configMINIMAL_STACK_SIZE,
  //             NULL, HIT_BIT_TASK_PRIORITY, NULL);
  // vCreateTask(hit_bit_task::vHitBitTask, "Task LCD",
  //             2 * configMINIMAL_STACK_SIZE, &pxLCDParameters,
  //             LCD_TASK_PRIORITY, NULL);

  vTaskStartScheduler();
  while (1)
    ;
}
