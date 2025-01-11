#include "C12832.h"
#include "CMD/cmd.h"
#include "LCD/LCD.h"
#include "LM75B.h"
#include "RTC.h"
#include "alarm_task.hpp"
#include "bubble_level_task.hpp"
#include "comando.hpp"
#include "configuration.hpp"
#include "global.h"
#include "hit_bit_task.hpp"
#include "lcd_task.hpp"
#include "max_min_task.hpp"
#include "mbed.h"
#include "pwm_task.hpp"
#include "tasks_macros.h"
#include "temperature_task.hpp"

Serial pc(USBTX, USBRX);

I2C i2c(p28, p27);

void check_tasks(void);

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

  xQueueMaxMin = xCreateQueue(MAX_MIN_TASK_QUEUE_SIZE,
                              sizeof(max_min_task::MaxMinMessage_t));
  xQueueAlarm =
      xCreateQueue(ALARM_TASK_QUEUE_SIZE, sizeof(alarm_task::AlarmMessage_t));
  xQueueLCD = xCreateQueue(LCD_TASK_QUEUE_SIZE, sizeof(lcd_task::LCDMessage_t));

  configuration::vConfigInitializer();
  scanI2CDevices();

  printf("Init complete..\n");

  vCreateTask(pwm_task::vPWMTask, "Task PWM", 2 * configMINIMAL_STACK_SIZE,
              NULL, MAX_MIN_TASK_PRIORITY, &xPWMHandler);

  vCreateTask(vMonitorTask, "Monitor", 2 * configMINIMAL_STACK_SIZE, NULL, 1,
              NULL);
  vCreateTask(temperature_task::vTemperatureTask, "Task Temperature",
              2 * configMINIMAL_STACK_SIZE, NULL, TEMPERATURE_TASK_PRIORITY,
              &xTemperatureHandler);
  vCreateTask(alarm_task::vAlarmTask, "Task Alarm",
              2 * configMINIMAL_STACK_SIZE, NULL,
              ALARM_TASK_PRIORITY, NULL);
  vCreateTask(max_min_task::vMaxMinTask, "Task Max Min",
              2 * configMINIMAL_STACK_SIZE, NULL,
              MAX_MIN_TASK_PRIORITY, NULL);
  vCreateTask(lcd_task::vLCDTask, "Task Hit Bit", 2 * configMINIMAL_STACK_SIZE,
              NULL, HIT_BIT_TASK_PRIORITY, NULL);
  vCreateTask(hit_bit_task::vHitBitTask, "Task LCD",
              2 * configMINIMAL_STACK_SIZE, NULL, LCD_TASK_PRIORITY,
              NULL);
  vCreateTask(bubble_level_task::vBubbleLevelTask, "Task Bubble Level",
              2 * configMINIMAL_STACK_SIZE, NULL, BUBBLE_LEVEL_TASK_PRIORITY,
              NULL);

  vTaskStartScheduler();
  while (1)
    ;
}
