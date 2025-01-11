
#ifndef __HIT_BIT_TASK_H
#define __HIT_BIT_TASK_H

namespace hit_bit_task {

  #define PLAY_MODE_UPDATE_TIME 1000
  #define WIN_MODE_UPDATE_TIME 1000
  #define JOYSTICK_DELAY_TIME 100

  bool xGetHitBitEnabled();
  void vSetHitBitEnabled(bool enabled);
  void vHitBitTask(void* pvParameters);
}

#endif // __HIT_BIT_TASK_H
