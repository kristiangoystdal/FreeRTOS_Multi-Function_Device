#include "global.h"

// Define the global queues
QueueHandle_t xQueueMaxMin;
QueueHandle_t xQueueAlarm;
QueueHandle_t xQueueLCD;
TaskHandle_t xPWMHandler;
TaskHandle_t xTemperatureHandler;

