
#ifndef __CONFIG_SOUND_TASK_H
#define __CONFIG_SOUND_TASK_H

#include "FreeRTOS.h"

namespace config_sound_task {

#define CONFIG_SOUND_UPDATE_TIME 200
#define TALA_DEFAULT_VALUE 5

typedef enum ConfigSoundAction { Alarm, Stop } ConfigSoundAction;

typedef struct {
  ConfigSoundAction xAction;
} ConfigSoundMessage_t;

bool xGetConfigSoundEnabled();
void vSetConfigSoundEnabled(bool enabled);

TickType_t xConfigGetTALA();
void vConfigSetTALA(int seconds);

void vConfigSoundTask(void *pvParameters);

} // namespace config_sound_task

#endif // __CONFIG_SOUND_TASK_H
