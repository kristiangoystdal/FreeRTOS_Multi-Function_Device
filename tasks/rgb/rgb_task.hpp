
#ifndef __RGB_TASK_HPP
#define __RGB_TASK_HPP

namespace rgb_task {

typedef enum RGBAction { Set, Update } RGBAction;

typedef struct RGBData {
  float r;
  float b;
  float g;
} RGBData;

typedef struct {
  RGBAction xAction;
  RGBData xData;
} RGBMessage_t;

void vRGBTask(void *pvParameters);

} // namespace rgb_task

#endif // __RGB_TASK_HPP
