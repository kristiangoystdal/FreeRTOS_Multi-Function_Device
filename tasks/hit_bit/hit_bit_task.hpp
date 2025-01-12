
#ifndef __HIT_BIT_TASK_H
#define __HIT_BIT_TASK_H

namespace hit_bit_task {

#define PLAY_MODE_UPDATE_TIME 1000
#define PLAY_MODE_DECREMENT_TIME 50
#define PLAY_MODE_MINIMUM_TIME 300
#define WIN_MODE_UPDATE_TIME 1000
#define JOYSTICK_DELAY_TIME 5

bool xGetHitBitEnabled();
void vSetHitBitEnabled(bool enabled);
void vHitBitTask(void *pvParameters);
} // namespace hit_bit_task

#endif // __HIT_BIT_TASK_H
