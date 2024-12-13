#include "C12832.h"
#include "LM75B.h"
#include "mbed.h"

C12832 lcd(p5, p7, p6, p8, p11);

LM75B sensor(p28, p27);
static Serial pc(USBTX, USBRX);

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
