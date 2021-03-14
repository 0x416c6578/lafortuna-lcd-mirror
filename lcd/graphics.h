#include "lcd.h"

void grInit(orientation orn, colour fg = WHITE, colour bg = BLACK);
void grClearDisplay();
void grFillRect(coord pos, uint16_t width, uint16_t height, colour col = WHITE);
void grWriteString(coord pos, char* string, colour fg = WHITE, colour bg = BLACK);
void grDrawRect(coord pos, uint16_t width, uint16_t height, uint8_t lineWidth, colour fg = WHITE, colour bg = BLACK);
void grDrawCircle(coord pos, uint16_t radius, uint16_t colour = WHITE);