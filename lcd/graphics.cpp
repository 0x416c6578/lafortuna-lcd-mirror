#include "graphics.h"

/*
Initialise graphics library (basically just init lcd for now)
*/
void grInit(orientation orn, colour fg, colour bg) {
  init_lcd(orn, fg, bg);
}

/*
Draw a rectangle outline with a given line width
*/
void grDrawRect(coord pos, uint16_t width, uint16_t height, uint8_t lineWidth, colour fg, colour bg) {
  if ((lineWidth >= width) | (lineWidth >= height)) {
    fill_rectangle_compat(pos, width, height, fg);
    return;
  }
  fill_rectangle_compat({pos.x, pos.y}, width, height, bg);
  //Top rect
  fill_rectangle_compat({pos.x, pos.y}, width, lineWidth, fg);
  //Bottom rect
  fill_rectangle_compat({pos.x, pos.y + height - lineWidth}, width, lineWidth, fg);
  //Left rect
  fill_rectangle_compat({pos.x, pos.y}, lineWidth, height, fg);
  //Right rect
  fill_rectangle_compat({pos.x + width - lineWidth, pos.y}, lineWidth, height, fg);
}

/*
Draw an outline of a circle using Bresenham's algorithm
*/
void grDrawCircle(coord pos, uint16_t radius, colour col) {
  int16_t x = 0;
  int16_t y = radius;
  int16_t decision = 3 - (2 * radius);
  DRAW_BRESENHAM_CIRCLE_FULL(pos.x, pos.y, x, y, col);
  while (y >= x) {
    x++;
    if (decision > 0) {
      y--;
      decision = decision + 4 * (x - y) + 10;
    } else {
      decision = decision + 4 * x + 6;
    }
    DRAW_BRESENHAM_CIRCLE_FULL(pos.x, pos.y, x, y, col);
  }
}

/*
Draw a horizontal line from `pos` with a given width
*/
void grDrawHLine(coord pos, uint16_t width, colour col) {
  fill_rectangle({pos.x, pos.x + width, pos.y, pos.y}, col);
}

/*
Draw a vertical line from `pos` with a given height
*/
void grDrawVLine(coord pos, uint16_t height, colour col) {
  fill_rectangle({pos.x, pos.x, pos.y, pos.y + height}, col);
}

/*
Draw a string with parameterised forground and background colours
*/
void grWriteString(coord pos, char* string, colour fg, colour bg) {
  display_string_xy(string, pos, fg, bg);
}

/*
Clear display using function from LCD driver
*/
void grClearDisplay() {
  clear_screen();
}

/*
Draw a rectangle at a given position with given width and height
(Default colour is white)
*/
void grFillRect(coord pos, uint16_t width, uint16_t height, colour col) {
  fill_rectangle_compat(pos, width, height, col);
}