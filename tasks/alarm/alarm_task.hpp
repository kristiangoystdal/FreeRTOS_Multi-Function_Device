
#ifndef __ALARM_TASK_H
#define __ALARM_TASK_H

#include "temperature_task.hpp"

namespace alarm_task {

  typedef enum AlarmAction {
    Get,
    SetClock,
    SetTempThreshold,
    SetClockEn,
    SetTempEn,
    Temp
  } AlarmAction;

  typedef struct {
    float tlow;
    float thigh;
    time_t tclock;
    bool clock_alarm_en;
    bool temp_alarm_en;
  } AlarmInfo_t;

  typedef struct {
    float tlow;
    float thigh; 
  } AlarmThreshold_t;

  typedef union AlarmData {
    time_t tclock;
    AlarmThreshold_t threshold;
    bool clock_alarm_en;
    bool temp_alarm_en;
    AlarmInfo_t xAlarmInfo;
    temperature_task::Measure_t xMeasure;
  } AlarmData;

  typedef struct {
    AlarmAction xAction;
    AlarmData xAlarmData;
  } AlarmMessage_t;
  
  void vAlarmTask(void* pvParameters);
}

#endif
