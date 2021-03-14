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
Draw an outline of a circle
*/
void grDrawCircle(coord pos, uint16_t radius, uint16_t colour) {
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x = 0;
  int16_t y = radius;

  draw_pixel({pos.x, pos.y + radius}, colour);
  draw_pixel({pos.x, pos.y - radius}, colour);
  draw_pixel({pos.x + radius, pos.y}, colour);
  draw_pixel({pos.x - radius, pos.y}, colour);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    draw_pixel({pos.x + x, pos.y + y}, colour);
    draw_pixel({pos.x - x, pos.y + y}, colour);
    draw_pixel({pos.x + x, pos.y - y}, colour);
    draw_pixel({pos.x - x, pos.y - y}, colour);
    draw_pixel({pos.x + y, pos.y + x}, colour);
    draw_pixel({pos.x - y, pos.y + x}, colour);
    draw_pixel({pos.x + y, pos.y - x}, colour);
    draw_pixel({pos.x - y, pos.y - x}, colour);
  }
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