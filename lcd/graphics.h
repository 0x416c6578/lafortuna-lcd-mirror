#include "lcd.h"
#define DRAW_BRESENHAM_CIRCLE_FULL(xc, yc, x, y, col) \
  draw_pixel({xc + x, yc + y}, col);                  \
  draw_pixel({xc - x, yc + y}, col);                  \
  draw_pixel({xc + x, yc - y}, col);                  \
  draw_pixel({xc - x, yc - y}, col);                  \
  draw_pixel({xc + y, yc + x}, col);                  \
  draw_pixel({xc - y, yc + x}, col);                  \
  draw_pixel({xc + y, yc - x}, col);                  \
  draw_pixel({xc - y, yc - x}, col)

void grInit(orientation orn, colour fg = WHITE, colour bg = BLACK);
void grClearDisplay();
void grFillRect(coord pos, uint16_t width, uint16_t height, colour col = WHITE);
void grWriteString(coord pos, char* string, colour fg = WHITE, colour bg = BLACK);
void grDrawRect(coord pos, uint16_t width, uint16_t height, uint8_t lineWidth, colour fg = WHITE, colour bg = BLACK);
void grDrawCircle(coord pos, uint16_t radius, colour col = WHITE);
void grDrawHLine(coord pos, uint16_t width, colour col);
void grDrawVLine(coord pos, uint16_t height, colour col);