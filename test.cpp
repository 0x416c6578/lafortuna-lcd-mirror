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
  char buf[4];
  sprintf(buf, "%02d", sizeof(uint32_t));
  grWriteString({0, 0}, buf);
}