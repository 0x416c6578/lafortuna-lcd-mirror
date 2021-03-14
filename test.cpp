#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/iousb1286.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "lcd/graphics.h"

int main() {
  //Clock setup:
  CLKPR = (1 << CLKPCE);
  CLKPR = 0;

  grInit(West, WHITE);  //Use default arguments for fg and bg colours
  //Create some abstract art
  while (1) {
    grDrawRect({rand() % 260, rand() % 180},
               rand() % 80, rand() % 80, rand() % 30,
               RGB_CONVERT(rand() & 0xff, rand() & 0xff, rand() & 0xff),
               RGB_CONVERT(rand() & 0xff, rand() & 0xff, rand() & 0xff));
  }
}