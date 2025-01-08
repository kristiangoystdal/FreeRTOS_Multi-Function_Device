
#ifndef __TASK_PRIORITIES_H
#define __TASK_PRIORITIES_H

#define CONSOLE_TASK_PRIORITY 1
#define RTC_GETTER_SETTER_PRIORITY 2
#define MAX_MIN_TASK_PRIORITY 2
#define ALARM_GETTER_SETTER_PRIORITY 2
#define BUBBLE_LEVEL_GETTER_SETTER_PRIORITY 2
#define HIT_BIT_GETTER_SETTER_PRIORITY 2
#define ALARM_TASK_PRIORITY 3
#define TEMPERATURE_TASK_PRIORITY 3
#define BUBBLE_LEVEL_TASK_PRIORITY 3
#define HIT_BIT_TASK_PRIORITY 3
#define LCD_TASK_PRIORITY 4
#define PWM_TASK_PRIORITY 4

#define CONSOLE_TASK_QUEUE_SIZE 10 // TODO: Think about this
#define ALARM_TASK_QUEUE_SIZE 2
#define MAX_MIN_TASK_QUEUE_SIZE 2
#define TEMPERATURE_TASK_QUEUE_SIZE 1
#define LCD_TASK_QUEUE_SIZE 3

#endif
