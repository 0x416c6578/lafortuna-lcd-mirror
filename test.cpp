#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/iousb1286.h>
#include <stdio.h>
#include <util/delay.h>

#include "lcd/graphics.h"

int main() {
  //Clock setup:
  CLKPR = (1 << CLKPCE);
  CLKPR = 0;

  grInit(West);                              //Use default arguments for fg and bg colours
  grDrawBox({0, 0}, 10, 10, 2, RED, GREEN);  //Draw a 100x100 box at 0,0 with line width 4
}