#include "C12832.h"
#include "CMD/cmd.h"
#include "FreeRTOS.h"
#include "LCD/LCD.h"
#include "LM75B.h"
#include "RTC.h"
#include "alarm_task.hpp"
#include "lcd_task.hpp"
#include "max_min_task.hpp"
#include "mbed.h"
#include "pwm_task.hpp"
#include "queue.h"
#include "task.h"
#include "tasks_macros.h"
#include "temperature_task.hpp"
#include "max_min_task.hpp"
#include "configuration.hpp"
#include "date_time.hpp"
#include "pwm_task.hpp"
#include "hit_bit_task.hpp"

Serial pc(USBTX, USBRX);

// LM75B sensor(p28, p27);
I2C i2c(p28, p27);

DigitalOut led(LED1);

QueueHandle_t xQueue;

void check_temperature(void);
void check_rtc(void);
void check_lcd(void);
void check_cmd(void);
void check_tasks(void);
extern void monitor(void);
QueueHandle_t createQueue(UBaseType_t uxSize, UBaseType_t uxType);

int main() {
  pc.baud(115200);
  int testNumber = 3; // TODO: Change this value for do the other tests
  switch (testNumber) {
  case 0:
    check_temperature();
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

// void check_temperature() {
//   while (1) {
//     // Try to open the LM75B
//     if (sensor.open()) {
//       printf("Device detected!\n");

//       while (1)
//         ;

//     } else {
//       error("Device not detected!\n");
//     }
//   }
// }

void ledFunction(void) {
  led = 1;
  RTC::detach(RTC::Second);
}

void displayFunction(void) {
  time_t seconds = time(NULL);
  printf("%s", ctime(&seconds));
    } else {
      error("Device not detected!\n");
    }
    // led2 = !led2;
  }
}

void alarmFunction(void) { error("Not most useful alarm function"); }

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

void scanI2CDevices() {
  printf("Scanning I2C bus...\n");
  for (int address = 0; address < 128; address++) {
    if (i2c.write(address << 1, NULL, 0) == 0) {
      printf("Device found at address 0x%X\n", address);
    }
  }
}

void check_tasks() {

  QueueHandle_t xQueueMaxMin = createQueue(
      MAX_MIN_TASK_QUEUE_SIZE, sizeof(max_min_task::MaxMinMessage_t));
  QueueHandle_t xQueueAlarm =
      createQueue(ALARM_TASK_QUEUE_SIZE, sizeof(alarm_task::AlarmMessage_t));
  QueueHandle_t xQueueLCD =
      createQueue(LCD_TASK_QUEUE_SIZE, sizeof(lcd_task::LCDMessage_t));
  QueueHandle_t xQueueConsole =
      createQueue(CONSOLE_TASK_QUEUE_SIZE, 100); // TODO: Think about this

  QueueHandle_t pxReadTemperatureParameters[4] = {xQueueMaxMin, xQueueAlarm,
                                                  xQueueLCD, xQueueConsole};
  QueueHandle_t pxMaxMinParameters[2] = {xQueueMaxMin, xQueueConsole};
  QueueHandle_t pxCmdParameters[4] = {xQueueConsole, xQueueMaxMin, xQueueAlarm,
                                      xQueueLCD};
  QueueHandle_t pxLCDParameters[4] = {xQueueLCD, xQueueMaxMin, xQueueAlarm,
                                      xQueueConsole};

  configuration::vConfigInitializer();
  max_min_task::vMaxMinInitialize();
  comando::vCommandInitialize(&pxCmdParameters);
  scanI2CDevices();
  setup_temp_sensor();

  printf("Init complete...");

  TaskHandle_t xReadTemperatureTaskHandler;
  init_TaskScheduler(&xQueue);

  xTaskCreate(vTask1, "Task 1", 2 * configMINIMAL_STACK_SIZE, xQueue, 20, NULL);
  // xTaskCreate(vTask2, "Task 2", 2 * configMINIMAL_STACK_SIZE,
  //   xQueue, 21, NULL);

  xTaskCreate(command_task::vCommandTask, "Task Command",
              4 * configMINIMAL_STACK_SIZE, &pxCmdParameters,
              CONSOLE_TASK_PRIORITY, NULL);
  xTaskCreate(temperature_task::vTemperatureTask, "Task Read Temperature",
              2 * configMINIMAL_STACK_SIZE, &pxReadTemperatureParameters,
              TEMPERATURE_TASK_PRIORITY, &xReadTemperatureTaskHandler);
  xTaskCreate(max_min_task::vMaxMinTask, "Task Max Min",
              2 * configMINIMAL_STACK_SIZE, &pxMaxMinParameters,
              MAX_MIN_TASK_PRIORITY, NULL);
  xTaskCreate(lcd_task::vLCDTask, "Task LCD", 2 * configMINIMAL_STACK_SIZE,
              &pxLCDParameters, LCD_TASK_PRIORITY, NULL);

  vTaskStartScheduler();
  while (1)
    ;
}

QueueHandle_t createQueue(UBaseType_t uxSize, UBaseType_t uxType) {
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
  QueueHandle_t pxLCDParameters [1] = {xQueueLCD};

  configuration::vConfigInitializer();

  TaskHandle_t xTemperatureTaskHandler;
  vCreateTask(temperature_task::vTemperatureTask, "Task Temperature", 2 * configMINIMAL_STACK_SIZE, &pxTemperatureParameters, TEMPERATURE_TASK_PRIORITY, &xTemperatureTaskHandler);
  vCreateTask(alarm_task::vAlarmTask, "Task Alarm", 2 * configMINIMAL_STACK_SIZE, &pxAlarmParameters , ALARM_TASK_PRIORITY, NULL);
  vCreateTask(max_min_task::vMaxMinTask, "Task Max Min", 2 * configMINIMAL_STACK_SIZE, &pxMaxMinParameters, MAX_MIN_TASK_PRIORITY, NULL);
  vCreateTask(lcd_task::vLCDTask, "Task Hit Bit", 2 * configMINIMAL_STACK_SIZE, NULL, HIT_BIT_TASK_PRIORITY, NULL);
  vCreateTask(hit_bit_task::vHitBitTask, "Task LCD", 2 * configMINIMAL_STACK_SIZE, &pxLCDParameters, LCD_TASK_PRIORITY, NULL);
  vTaskStartScheduler();
  while(1);
}
