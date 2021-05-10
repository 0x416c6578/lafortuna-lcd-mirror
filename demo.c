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

  grInit(West, WHITE, BLACK);  //Use default arguments for fg and bg colours

  grWriteString((coord){0, 0}, "Demonstration of LaFortuna-LCD", WHITE, BLACK);
  grWriteString((coord){0, 10}, "grDrawLine", WHITE, BLACK);
  grDrawLine((coord){0, 20}, (coord){50, 30}, GREEN);

  grWriteString((coord){80, 10}, "grDrawRect", WHITE, BLACK);
  grDrawRect((coord){80, 20}, 50, 10, 3, GREEN, BLACK);

  grWriteString((coord){160, 10}, "grDrawRoundedRect", WHITE, BLACK);
  grDrawRoundedRect((coord){160, 20}, 50, 10, 5, GREEN);

  grWriteString((coord){0, 35}, "grFillRect", WHITE, BLACK);
  grFillRect((coord){0, 45}, 50, 10, GREEN);

  grWriteString((coord){80, 35}, "grFillRoundedRect", WHITE, BLACK);
  grFillRoundedRect((coord){80, 45}, 50, 10, 5, GREEN);

  grWriteString((coord){200, 35}, "grDrawCircle", WHITE, BLACK);
  grDrawCircle((coord){210, 53}, 7, GREEN);

  grWriteString((coord){0, 65}, "grFillCircle", WHITE, BLACK);
  grFillCircle((coord){8, 83}, 7, GREEN);

  grWriteString((coord){80, 65}, "grDrawTriangle", WHITE, BLACK);
  grDrawTriangle((coord){80, 75}, (coord){85, 100}, (coord){100, 80}, GREEN);

  grWriteString((coord){200, 65}, "grFillTriangle", WHITE, BLACK);
  grFillTriangle((coord){200, 75}, (coord){205, 100}, (coord){220, 80}, GREEN);

  grWriteString((coord){0, 115}, "Now some coloured text", GREEN, BLUE);
  grWriteString((coord){0, 125}, "And some more coloured text", BLACK, YELLOW);

  grWriteString((coord){0, 145}, "Using RGB_CONVERT(255, 128, 128)", WHITE, BLACK);
  grFillRoundedRect((coord){0, 155}, 50, 15, 8, RGB_CONVERT(255, 128, 128));
}