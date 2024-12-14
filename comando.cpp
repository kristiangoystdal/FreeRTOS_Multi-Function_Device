// #ifdef notdef

/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (exemplo)
|
| Autor: Carlos Almeida (IST)
| Data:  Nov 2002
***************************************************************************/
#include "FreeRTOS.h"
// #include "global_queues.h"

#include "queue.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  int32_t lValueToSend;
  // BaseType_t xStatus;

  if (argc == 2) {
    printf("msg: %s\n", argv[1]);
    lValueToSend = atoi(argv[1]);
    // xStatus = xQueueSend(xQueue, &lValueToSend, 0);
  } else {
    printf("wrong number of arguments!\n");
  }
}

bool check_args_digit(char **argv) {
  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < strlen(argv[i]); j++) {
      if (!isdigit(argv[i][j])) {
        return false;
      }
    }
  }
  return true;
}

void cmd_rdt(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_rdt\n");
}

void cmd_sd(int argc, char **argv) {
  if (argc == 4) {
    if (check_args_digit(argv)) {
      printf("cmd_sd %d %d %d\n", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    } else {
      printf("wrong type of arguments!\n");
    }
    // Placeholder for command
  } else {

    printf("wrong number of arguments!\n");
  }
}

void cmd_rc(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_rc\n");
}

void cmd_sc(int argc, char **argv) {
  if (argc == 4) {
    // Placeholder for command
    printf("cmd_sc %d %d %d\n", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
  } else {
    printf("wrong number of arguments!\n");
  }
}

void cmd_rt(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_rt\n");
}

void cmd_rmm(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_rmm\n");
}

void cmd_cmm(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_cmm\n");
}

void cmd_rp(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_rp\n");
}

void cmd_mmp(int argc, char **argv) {
  if (argc == 2) {
    // Placeholder for command
    printf("cmd_mmp %d\n", atoi(argv[1]));
  } else {
    printf("wrong number of arguments!\n");
  }
}

void cmd_mta(int argc, char **argv) {
  if (argc == 2) {
    // Placeholder for command
    printf("cmd_mta %d\n", atoi(argv[1]));
  } else {
    printf("wrong number of arguments!\n");
  }
}

void cmd_rai(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_rai\n");
}

void cmd_sac(int argc, char **argv) {
  if (argc == 4) {
    // Placeholder for command
    printf("cmd_sac %d %d %d\n", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
  } else {
    printf("wrong number of arguments!\n");
  }
}

void cmd_sat(int argc, char **argv) {
  if (argc == 3) {
    // Placeholder for command
    printf("cmd_sat %d %d\n", atoi(argv[1]), atoi(argv[2]));
  } else {
    printf("wrong number of arguments!\n");
  }
}

void cmd_adac(int argc, char **argv) {
  if (argc == 2) {
    // Placeholder for command
    printf("cmd_adac %d\n", atoi(argv[1]));
  } else {
    printf("wrong number of arguments!\n");
  }
}

void cmd_adat(int argc, char **argv) {
  if (argc == 2) {
    // Placeholder for command
    printf("cmd_adat %d\n", atoi(argv[1]));
  } else {
    printf("wrong number of arguments!\n");
  }
}

void cmd_rts(int argc, char **argv) {
  // Placeholder for command
  printf("cmd_rts\n");
}

void cmd_adbl(int argc, char **argv) {
  if (argc == 2) {
    // Placeholder for command
    printf("cmd_adbl %d\n", atoi(argv[1]));
  } else {
    printf("wrong number of arguments!\n");
  }
}

void cmd_adhb(int argc, char **argv) {
  if (argc == 2) {
    // Placeholder for command
    printf("cmd_adhb %d\n", atoi(argv[1]));
  } else {
    printf("wrong number of arguments!\n");
  }
}

void cmd_adcs(int argc, char **argv) {
  if (argc == 2) {
    // Placeholder for command
    printf("cmd_adcs %d\n", atoi(argv[1]));
  } else {

    printf("wrong number of arguments!\n");
  }
}

/*-------------------------------------------------------------------------+

// #endif //notdef

*/