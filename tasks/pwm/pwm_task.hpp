
#ifndef __PWM_TASK_H
#define __PWM_TASK_H

namespace pwm_task {

  #define CONFIG_SOUND_UPDATE_TIME 200

  bool xGetConfigSoundEnabled();

  void vSetConfigSoundEnabled(bool enabled);

  void vPWMTask(void* pvParameters);

}

#endif // __PWM_TASK_H
