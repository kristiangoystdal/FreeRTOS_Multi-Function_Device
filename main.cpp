#include "C12832.h"
#include "FreeRTOS.h"
#include "LM75B.h"
#include "mbed.h"
#include "queue.h"

C12832 lcd(p5, p7, p6, p8, p11);

LM75B sensor(p28, p27);
Serial pc(USBTX, USBRX);

QueueHandle_t xQueue;

extern void monitor(void);

/*-------------------------------------------------------------------------+
| Function: my_fgets        (called from my_getline / monitor)
+--------------------------------------------------------------------------*/
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

int main() {
  pc.baud(115200);

  while (1) {
    // Try to open the LM75B
    if (sensor.open()) {
      printf("Device detected!\n");

      while (1) {
        lcd.cls();
        lcd.locate(0, 3);
        lcd.printf("Temp = %.3f\n", sensor.temp());
        wait(1.0);
      }

    } else {
      error("Device not detected!\n");
    }
  }
}
