// #ifdef notdef

/***************************************************************************
| File: monitor.c
|
| Autor: Carlos Almeida (IST), from work by Jose Rufino (IST/INESC),
|        from an original by Leendert Van Doorn
| Data:  Nov 2002
***************************************************************************/
#include "comando.hpp"
#include "mbed.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-------------------------------------------------------------------------+
| Headers of command functions
+--------------------------------------------------------------------------*/
void cmd_sos(int, char **);

/*-------------------------------------------------------------------------+
| Variable and constants definition
+--------------------------------------------------------------------------*/
const char TitleMsg[] = "\n Application Control Monitor\n";
const char InvalMsg[] = "\nInvalid command!";

struct command_d {
  void (*cmd_fnct)(int, char **);
  char *cmd_name;
  char *cmd_help;
} const commands[] = {
    {cmd_sos, "sos", "                             help"},
    {comando::cmd_send, "send", "<msg>                       send message"},
    {comando::cmd_sair, "sair", "                            sair"},
    {comando::cmd_test, "test", "<arg1> <arg2>               test command"},
    {comando::cmd_rdt, "rdt",
     "                             read date/time (dd/MM/YYYY hh:mm:ss)"},
    {comando::cmd_sd, "sd",
     "  <day> <month> <year>        set date (day, month, year)"},
    {comando::cmd_rc, "rc", "                              read clock"},
    {comando::cmd_sc, "sc", "  <hours> <minutes> <seconds> set clock"},
    {comando::cmd_rt, "rt", "                              read temperature"},
    {comando::cmd_rmm, "rmm",
     "                             read maximum and minimum of temperature"},
    {comando::cmd_cmm, "cmm",
     "                             clear maximum and minimum of temperature"},
    {comando::cmd_rp, "rp",
     "                              read parameters (pmon, tala)"},
    {comando::cmd_mmp, "mmp",
     " <p>                         modify monitoring period (seconds - 0 "
     "deactivate)"},
    {comando::cmd_mta, "mta",
     " <t>                         modify time alarm (seconds)"},
    {comando::cmd_rai, "rai",
     "                             read alarm info (alarm clock, tlow, thigh, "
     "active/inactive)"},
    {comando::cmd_sac, "sac",
     " <h> <m> <s>                 set alarm clock (hours, minutes, seconds)"},
    {comando::cmd_sat, "sat",
     " <tlow> <thigh>              set alarm temperature thresholds (tlow, "
     "thigh)"},
    {comando::cmd_adac, "adac",
     "1/0                         activate/deactivate alarm clock"},
    {comando::cmd_adat, "adat",
     "1/0                         activate/deactivate alarm temperature"},
    {comando::cmd_rts, "rts",
     "                             read task state (Bubble Level, Hit Bit, "
     "Config Sound)"},
    {comando::cmd_adbl, "adbl",
     "1/0                         activate/deactivate Bubble Level"},
    {comando::cmd_adhb, "adhb",
     "1/0                         activate/deactivate Hit Bit"},
    {comando::cmd_adcs, "adcs",
     "1/0                         activate/deactivate Config Sound"},
};

#define NCOMMANDS (sizeof(commands) / sizeof(struct command_d))
#define ARGVECSIZE 4
#define MAX_LINE 50

static Serial pc(USBTX, USBRX);

char *my_fgets(char *ln, int sz, FILE *f) {
  //  fgets(line, MAX_LINE, stdin);
  //  pc.gets(line, MAX_LINE);
  int i;
  char c;
  for (i = 0; i < sz - 1; i++) {
    c = pc.getc();
    ln[i] = c;
    if ((c == '\n') || (c == '\r'))
      break;
  }
  ln[i] = '\0';

  return ln;
}

/*-------------------------------------------------------------------------+
| Function: cmd_sos - provides a rudimentary help
+--------------------------------------------------------------------------*/
void cmd_sos(int argc, char **argv) {
  int i;

  printf("%s\n", TitleMsg);
  for (i = 0; i < NCOMMANDS; i++)
    printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
}

/*-------------------------------------------------------------------------+
| Function: my_getline        (called from monitor)
+--------------------------------------------------------------------------*/
int my_getline(char **argv, int argvsize) {
  static char line[MAX_LINE];
  char *p;
  int argc;

  //  fgets(line, MAX_LINE, stdin);
  my_fgets(line, MAX_LINE, stdin);

  /* Break command line into an o.s. like argument vector,
     i.e. compliant with the (int argc, char **argv) specification --------*/

  for (argc = 0, p = line; (*line != '\0') && (argc < argvsize);
       p = NULL, argc++) {
    p = strtok(p, " \t\n");
    argv[argc] = p;
    if (p == NULL)
      return argc;
  }
  argv[argc] = p;
  return argc;
}

/*-------------------------------------------------------------------------+
| Function: monitor        (called from main)
+--------------------------------------------------------------------------*/
void vMonitorTask(void *pvParameters) {
  static char *argv[ARGVECSIZE + 1], *p;
  int argc, i;

  printf("%s Type sos for help\n", TitleMsg);
  for (;;) {
    printf("\nCmd> ");
    /* Reading and parsing command line  ----------------------------------*/
    if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
      for (p = argv[0]; *p != '\0'; *p = tolower(*p), p++)
        ;
      for (i = 0; i < NCOMMANDS; i++)
        if (strcmp(argv[0], commands[i].cmd_name) == 0)
          break;
      /* Executing commands -----------------------------------------------*/
      if (i < NCOMMANDS)
        commands[i].cmd_fnct(argc, argv);
      else
        printf("%s", InvalMsg);
    } /* if my_getline */
  } /* forever */
}

// #endif //notdef
