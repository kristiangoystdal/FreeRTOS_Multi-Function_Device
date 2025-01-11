
/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (exemplo)
|
| Autor: Carlos Almeida (IST)
| Data:  Nov 2002
***************************************************************************/
#include "comando.hpp"
#include "FreeRTOS.h"
#include "LM75B.h"
#include "alarm_task.hpp"
#include "bubble_level_task.hpp"
#include "configuration.hpp"
#include "date_time.hpp"
#include "global.h"
#include "hit_bit_task.hpp"
#include "lcd_task.hpp"
#include "max_min_task.hpp"
#include "pwm_task.hpp"
#include "queue.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace comando {

/*-------------------------------------------------------------------------+
| Function: cmd_sair - termina a aplicacao
+--------------------------------------------------------------------------*/
void cmd_sair(int argc, char **argv) {
  //  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/
void cmd_test(int argc, char **argv) {
  int i;

  /* exemplo -- escreve argumentos */
  for (i = 0; i < argc; i++)
    printf("\nargv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_send - send message
+--------------------------------------------------------------------------*/
void cmd_send(int argc, char **argv) {
  //   BaseType_t xStatus;

  if (argc == 2) {
    printf("msg: %s\n", argv[1]);
    // xStatus = xQueueSend(xQueue, &lValueToSend, 0);
  } else {
    printf("wrong number of arguments!\n");
  }
}

bool check_args_digit(char **argv) {
  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < strlen(argv[i]); j++) {
      if (!isdigit(argv[i][j])) {
        return true;
      }
    }
  }
  return false;
}

bool check_args_range(char **argv, int ranges[][2]) {
  for (int i = 1; i < 4; i++) {
    int value = atoi(argv[i]);
    if (value <= ranges[i - 1][0] && value >= ranges[i - 1][1]) {
      return true;
    }
  }
  return false;
}

bool check_args(int argc, char **argv, int num_args, int ranges[][2]) {
  if (argc != num_args) {
    printf("wrong number of arguments!\n");
    return true;
  }
  if (check_args_digit(argv)) {
    printf("wrong type of arguments!\n");
    return true;
  }
  if (check_args_range(argv, ranges)) {
    printf("wrong range of arguments!\n");
    return true;
  }
  return false;
}

/*-------------------------------------------------------------------------+
| Ranges for the arguments
+--------------------------------------------------------------------------*/
int ranges_dates[3][2] = {{0, 31}, {0, 12}, {0, 9999}};
int ranges_time[3][2] = {{0, 23}, {0, 59}, {0, 59}};
int ranges_temp[1][2] = {{0, 50}};
int ranges_period[1][2] = {{0, 99}};
int ranges_alarm[1][2] = {{0, 60}};
int ranges_bool[1][2] = {{0, 1}};

/*-------------------------------------------------------------------------+
| Functions for the commands
+--------------------------------------------------------------------------*/

void cmd_rdt(int argc, char **argv) {
  time_t xTime = date_time::get_time();
  char buffer[20];
  date_time::convertTimeToDateClockString(xTime, buffer, sizeof(buffer));
  printf("%s\n", buffer);
}

void cmd_sd(int argc, char **argv) {
  if (check_args(argc, argv, 4, ranges_dates)) {
    return;
  }
  char s[20];
  char buffer[10];
  date_time::get_clock(buffer);
  snprintf(s, sizeof(s), "%02d/%02d/%04d %s", atoi(argv[1]), atoi(argv[2]),
           atoi(argv[3]), buffer);
  date_time::set_dateTime(s);
}

void cmd_rc(int argc, char **argv) {
  char s[20];
  date_time::get_clock(s);
  printf("%s\n", s);
}

void cmd_sc(int argc, char **argv) {
  if (check_args(argc, argv, 4, ranges_time)) {
    return;
  }

  char s[20];
  date_time::get_date(s);

  snprintf(s + 11, sizeof(s) - 11, "%02d:%02d:%02d", atoi(argv[1]),
           atoi(argv[2]), atoi(argv[3]));

  date_time::set_dateTime(s);
}

void cmd_rt(int argc, char **argv) {
  float temp;
  temperature_task::get_temperature(&temp);
  printf("Temperature: %.1f\n", temp);
}

void cmd_rmm(int argc, char **argv) {
  // Placeholder for command
  max_min_task::MaxMinMessage_t xMaxMinMessage;
  xMaxMinMessage.xAction = max_min_task::Get;
  xMaxMinMessage.xMeasure.xTemp = 0;
  xMaxMinMessage.xMeasure.xTime = 0;
  BaseType_t xStatus = xQueueSend(xQueueMaxMin, &xMaxMinMessage, 0);
  if (xStatus == errQUEUE_FULL) {
    printf("ERROR: Queue full: Temperature -> Max/Min");
  }
}

void cmd_cmm(int argc, char **argv) { temperature_task::vMaxMinInitialize(); }

void cmd_rp(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_rp\n");
}

void cmd_mmp(int argc, char **argv) {
  if (check_args(argc, argv, 2, ranges_period)) {
    return;
  }

  printf("cmd_mmp %d\n", atoi(argv[1]));
}

void cmd_mta(int argc, char **argv) {
  if (check_args(argc, argv, 2, ranges_alarm)) {
    return;
  }

  printf("cmd_mta %d\n", atoi(argv[1]));
}

void cmd_rai(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_rai\n");
}

void cmd_sac(int argc, char **argv) {
  if (check_args(argc, argv, 4, ranges_temp)) {
    return;
  }

  // Placeholder for command
  printf("cmd_sac %d %d %d\n", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
}

void cmd_sat(int argc, char **argv) {
  int ranges_temp_double[2][2] = {{ranges_temp[0][0], ranges_temp[0][1]},
                                  {ranges_temp[0][0], ranges_temp[0][1]}};
  if (check_args(argc, argv, 3, ranges_temp_double)) {
    return;
  }

  // Placeholder for command
  printf("cmd_sat %d %d\n", atoi(argv[1]), atoi(argv[2]));
}

void cmd_adac(int argc, char **argv) {
  if (check_args(argc, argv, 2, ranges_bool)) {
    return;
  }
  alarm_task::AlarmMessage_t xAlarmMessage;
  xAlarmMessage.xAction = alarm_task::SetClockEn;
  xAlarmMessage.xAlarmData.clock_alarm_en = atoi(argv[1]);
  BaseType_t xStatus = xQueueSend(xQueueAlarm, &xAlarmMessage, 0);
  if (xStatus == errQUEUE_FULL) {
    printf("ERROR: Queue full: QueueAlarm");
  } else {
    printf("Pass\n");
  }
  printf("cmd_adac %d\n", atoi(argv[1]));
}

void cmd_adat(int argc, char **argv) {
  if (check_args(argc, argv, 2, ranges_bool)) {
    return;
  }
  alarm_task::AlarmMessage_t xAlarmMessage;
  xAlarmMessage.xAction = alarm_task::SetTempEn;
  xAlarmMessage.xAlarmData.temp_alarm_en = atoi(argv[1]);
  BaseType_t xStatus = xQueueSend(xQueueAlarm, &xAlarmMessage, 0);
  if (xStatus == errQUEUE_FULL) {
    printf("ERROR: Queue full: QueueAlarm");
  } else {
    printf("Pass\n");
  }
  printf("cmd_adat %d\n", atoi(argv[1]));
}

void cmd_rts(int argc, char **argv) {
  bool bubble_level_en = bubble_level_task::xGetBubbleLevelEnabled();
  bool hit_bit_en = hit_bit_task::xGetHitBitEnabled();
  bool config_sound_en = pwm_task::xGetConfigSoundEnabled();
  printf("%d, %d, %d\n", bubble_level_en, hit_bit_en, config_sound_en);
  printf("cmd_rts\n");
}

void cmd_adbl(int argc, char **argv) {
  if (check_args(argc, argv, 2, ranges_bool)) {
    return;
  }

  bubble_level_task::vSetBubbleLevelEnabled((bool)atoi(argv[1]));
  printf("cmd_adbl %d\n", atoi(argv[1]));
}

void cmd_adhb(int argc, char **argv) {
  if (check_args(argc, argv, 2, ranges_bool)) {
    return;
  }

  hit_bit_task::vSetHitBitEnabled((bool)atoi(argv[1]));
  printf("cmd_adhb %d\n", atoi(argv[1]));
}

void cmd_adcs(int argc, char **argv) {
  if (check_args(argc, argv, 2, ranges_bool)) {
    return;
  }

  pwm_task::vSetConfigSoundEnabled((bool)atoi(argv[1]));
  printf("cmd_adcs %d\n", atoi(argv[1]));
}
} // namespace comando
