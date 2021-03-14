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
  //grDrawRect({0, 0}, 100, 100, 10, RED, GREEN);  //Draw a 100x100 box at 0,0 with line width 4
  //grWriteString({0, 150}, "This is a test string\nWith a newline", RED, GREEN);
  for (int i = 1; i < 150; i++) {
    grDrawCircle({160, 120}, i);
  }
}