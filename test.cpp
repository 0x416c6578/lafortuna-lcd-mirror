#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/iousb1286.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "lcd/graphics.h"

uint16_t frameNum = 0;

int main() {
  //Clock setup:
  CLKPR = (1 << CLKPCE);
  CLKPR = 0;

  grInit(West, WHITE);  //Use default arguments for fg and bg colours

  uint16_t i = 0;
  for (;;) {
    if (i == 320) {
      grFillRect({i, 120}, 5, 5, BLACK);
      i = 0;
    }
    grFillRect({i, 120}, 5, 5, BLACK);
    grFillRect({i + 1, 120}, 5, 5, WHITE);
    i++;
    _delay_ms(2);
  }
  grDrawCircle({160, 120}, 20);
}