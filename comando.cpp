//#ifdef notdef

/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (exemplo)
|
| Autor: Carlos Almeida (IST)
| Data:  Nov 2002
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "GlobalQueues.h"

/*-------------------------------------------------------------------------+
| Function: cmd_sair - termina a aplicacao
+--------------------------------------------------------------------------*/ 
void cmd_sair (int argc, char **argv)
{
//  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_test (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\nargv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_send - send message
+--------------------------------------------------------------------------*/ 
void cmd_send (int argc, char** argv)
{
int32_t lValueToSend;
BaseType_t xStatus;

    if (argc == 2) {
        printf ("msg: %s\n", argv[1]);
        lValueToSend = atoi(argv[1]);
        xStatus = xQueueSend( xQueue, &lValueToSend, 0 );
    }
    else {
        printf ("wrong number of arguments!\n");
    }
}

//#endif //notdef
