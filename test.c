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

  grInit(East, WHITE, BLACK);  //Use default arguments for fg and bg colours

  grFillRect((coord){0, 0}, 320, 240, GREEN);
}