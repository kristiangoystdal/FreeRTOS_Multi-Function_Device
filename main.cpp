#include "C12832.h"
#include "LCD/LCD.h"
#include "RTC.h"
#include "alarm_task.hpp"
#include "bubble_level_task.hpp"
#include "configuration.hpp"
#include "date_time.hpp"
#include "global.h"
#include "hit_bit_task.hpp"
#include "lcd_task.hpp"
#include "max_min_task.hpp"
#include "mbed.h"
#include "monitor_task.hpp"
#include "config_sound_task.hpp"
#include "tasks_macros.h"
#include "temperature_task.hpp"

I2C i2c(p28, p27);

void check_tasks(void);

int main() {
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

void check_tasks() {
  monitor_task::vInitPC();
  xQueueMaxMin = xCreateQueue(MAX_MIN_TASK_QUEUE_SIZE,
                              sizeof(max_min_task::MaxMinMessage_t));
  xQueueAlarm =
      xCreateQueue(ALARM_TASK_QUEUE_SIZE, sizeof(alarm_task::AlarmMessage_t));
  xQueueLCD = xCreateQueue(LCD_TASK_QUEUE_SIZE, sizeof(lcd_task::LCDMessage_t));
  xQueueConfigSound = xCreateQueue(CONFIG_SOUND_TASK_PRIORITY, sizeof(config_sound_task::ConfigSoundMessage_t));

  configuration::vConfigInitializer();
  NVIC_SetPriority(RTC_IRQn, 254);
  NVIC_SetPriority(EINT3_IRQn, 253);
  RTC::attach(&lcd_task::vUpdateClockISR, RTC::Second);
  scanI2CDevices();

  printf("Init complete..\n");

  vCreateTask(config_sound_task::vConfigSoundTask, "Task Config Sound", 2 * configMINIMAL_STACK_SIZE,
              NULL, MAX_MIN_TASK_PRIORITY, NULL);
  vCreateTask(monitor_task::vMonitorTask, "Monitor", 2 * configMINIMAL_STACK_SIZE, NULL,
              MONITOR_TASK_PRIORITY, NULL);
  vCreateTask(temperature_task::vTemperatureTask, "Task Temperature",
              2 * configMINIMAL_STACK_SIZE, NULL, TEMPERATURE_TASK_PRIORITY,
              &xTemperatureHandler);
  vCreateTask(alarm_task::vAlarmTask, "Task Alarm",
              2 * configMINIMAL_STACK_SIZE, NULL, ALARM_TASK_PRIORITY, NULL);
  vCreateTask(max_min_task::vMaxMinTask, "Task Max Min",
              2 * configMINIMAL_STACK_SIZE, NULL, MAX_MIN_TASK_PRIORITY, NULL);
  vCreateTask(hit_bit_task::vHitBitTask, "Task Hit Bit",
              2 * configMINIMAL_STACK_SIZE, NULL, HIT_BIT_TASK_PRIORITY,
              &xHitBitHandler);
  vCreateTask(lcd_task::vLCDTask, "Task LCD", 2 * configMINIMAL_STACK_SIZE,
              NULL, LCD_TASK_PRIORITY, NULL);
  vCreateTask(bubble_level_task::vBubbleLevelTask, "Task Bubble Level",
              2 * configMINIMAL_STACK_SIZE, NULL, BUBBLE_LEVEL_TASK_PRIORITY,
              &xBubbleLevelHandler);

  vTaskStartScheduler();
  while (1)
    ;
}
