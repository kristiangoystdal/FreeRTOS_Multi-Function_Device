
#ifndef __CONFIG_SOUND_TASK_H
#define __CONFIG_SOUND_TASK_H

namespace config_sound_task {

  #define CONFIG_SOUND_UPDATE_TIME 200

  bool xGetConfigSoundEnabled();

  void vSetConfigSoundEnabled(bool enabled);

  void vConfigSoundTask(void* pvParameters);

}

#endif // __CONFIG_SOUND_TASK_H
