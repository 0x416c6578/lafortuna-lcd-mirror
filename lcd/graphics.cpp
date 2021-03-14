#include "graphics.h"

/*
Initialise graphics library (basically just init lcd for now)
*/
void grInit(orientation orn, colour fg, colour bg){
  init_lcd(orn, fg, bg);
}

/*
Draw a rectangle outline with a given line width
*/
void grDrawBox(coord pos, uint16_t width, uint16_t height, uint8_t lineWidth, colour fg, colour bg) {
  if ((lineWidth >= width) | (lineWidth >= height)){
    fill_rectangle_compat(pos, width, height, fg);
    return;
  }
  //Clear rect of stuff
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