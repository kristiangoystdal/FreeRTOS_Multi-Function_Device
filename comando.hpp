// Command.h
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "FreeRTOS.h"
#include "queue.h"

namespace comando {
void vCommandInitialize(QueueHandle_t (*pxQueueArray)[4]);
}

#endif
