#ifndef COMANDO_HPP
#define COMANDO_HPP

#include "FreeRTOS.h"
#include "queue.h"

namespace comando {
void vCommandInitialize(QueueHandle_t pxQueueArray);

} // namespace comando

#endif // COMANDO_HPP
