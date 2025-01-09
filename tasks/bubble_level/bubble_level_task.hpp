
#ifndef __BUBBLE_LEVEL_TASK_H
#define __BUBBLE_LEVEL_TASK_H

namespace bubble_level_task {

  #define BUBBLE_LEVEL_UPDATE_TIME 33
  
  bool xGetBubbleLevelEnabled();

  void vSetBubbleLevelEnabled(bool enabled);

  void vBubbleLevelTask(void* pvParameters);

}

#endif // __BUBBLE_LEVEL_TASK_H
