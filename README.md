# Overview
- This is my attempt at understanding and documenting everything in the LCD driver as well as writing a simple graphics library
- Currently the main driver is comprehensively commented with as much as I can work out
- Code is based on the Adafruit library, detailed [here](http://forum.pjrc.com/threads/26305-Highly-optimized-ILI9341-%28320x240-TFT-color-display%29-library)

# Documentation
## Usage
- `#include "lcd/graphics.h"`
- All graphics functions are prepended with "gr"
- Some functions, for example grFillRect are just wrappers around the `lcd.h` functions
- The `coord` struct is used to wrap an x and y coordinate (accessed with `(coord) c.x` and `(coord) c.y`)
- To initialise the display, call `grInit(orientation orn, colour fg, colour bg)`
  - This takes an orientation and a default foreground and background colour for the display (unused)
  - This must be done before any of the other graphics functions are called

## Lines
- `grDrawLine(coord p1, coord p2, colour color)` takes two `coord`s and a colour
- It draws a line from the first `coord` to the second
- `grDrawHLine(coord pos, uint16_t width, colour col)` and `grDrawVLine(coord pos, uint16_t height, colour col)` draw horizontal and vertical lines respectively
  - Both take a `coord` of the starting point, and a width or height accordingly (as well as a colour)

## Rectangles
- `grDrawRect(coord pos, uint16_t width, uint16_t height, uint8_t lineWidth, colour fg, colour bg)` takes an origin position `coord`, a width and a height, a line draw width and a foreground and background colour
- The rectangle drawn has its top-left corner in the origin position, and the line width is determined by the `lineWidth` argument
- `grFillRect(coord pos, uint16_t width, uint16_t height, colour col)` takes an origin position `coord`, a width and height and a colour, and draws a filled rectangle similar to `grDrawRect`
- `grDrawRoundedRect(coord pos, uint16_t width, uint16_t height, uint16_t radius, colour col)` is similar to `grDrawRect` but it takes a `radius` argument which determines the radius of the rectangles corners
- `grFillRoundedRect(coord pos, uint16_t width, uint16_t height, uint16_t radius, colour col)` is equivalent to above except the rectangle is filled

## Circles
- `grDrawCircle(coord pos, uint16_t radius, colour col)` takes a center position `coord`, a radius and a colour
- `grFillCircle(coord pos, uint16_t radius, colour col)` is equivalent to above except the circle is filled

## Triangles
- `grDrawTriangle(coord p1, coord p2, coord p3, colour col)` takes three points and a colour, and draws a triangle accordingly
- `grFillTriangle(coord p1, coord p2, coord p3, colour color)` is equivalent to above except it fills the triangle

## Text
- `grWriteString(coord pos, char* string, colour fg, colour bg)` takes an origin position (again top-left corner of text), a `char*` containing the text, and a foreground and background colour for the text

## Other Stuff
- `grClearDisplay()` clears the display
- `RGB_CONVERT(r, g, b)` takes three bytes for red, green and blue and converts them to a `colour` (`uint16_t`) for use with the display
