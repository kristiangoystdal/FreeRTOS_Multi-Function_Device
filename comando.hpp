#ifndef COMANDO_HPP
#define COMANDO_HPP

#include "FreeRTOS.h"
#include "queue.h"

namespace comando {
// Initialization and task-related functions
void vCommandInitialize(QueueHandle_t pxQueueArray[4]);
void vCommandTask(void *pvParameters);

// Command functions
void cmd_sair(int argc, char **argv);
void cmd_test(int argc, char **argv);
void cmd_send(int argc, char **argv);
void cmd_rdt(int argc, char **argv);
void cmd_sd(int argc, char **argv);
void cmd_rc(int argc, char **argv);
void cmd_sc(int argc, char **argv);
void cmd_rt(int argc, char **argv);
void cmd_rmm(int argc, char **argv);
void cmd_cmm(int argc, char **argv);
void cmd_rp(int argc, char **argv);
void cmd_mmp(int argc, char **argv);
void cmd_mta(int argc, char **argv);
void cmd_rai(int argc, char **argv);
void cmd_sac(int argc, char **argv);
void cmd_sat(int argc, char **argv);
void cmd_adac(int argc, char **argv);
void cmd_adat(int argc, char **argv);
void cmd_rts(int argc, char **argv);
void cmd_adbl(int argc, char **argv);
void cmd_adhb(int argc, char **argv);
void cmd_adcs(int argc, char **argv);
} // namespace comando

#endif // COMANDO_HPP
