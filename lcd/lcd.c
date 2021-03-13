/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *           View this license at http://creativecommons.org/about/licenses/
 *
 *
 *  - Jan 2015  Modified for LaFortuna (Rev A, black edition) [KPZ]
 */

#include "lcd.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/iousb1286.h>
#include <stdint.h>
#include <util/delay.h>

#include "font.h"
#include "ili934x.h"

/*
uint16_t width, height;                 - Display width and height
orientation orient;                     - Display orientation
uint16_t x, y;                          - Display write-to point (used for writing characters and strings)
uint16_t foreground, background;        - Default forground and background colours
*/
lcd display;

#define set_ram_write_region(sco, eco, spg, epg) \
  write_cmd(COLUMN_ADDRESS_SET);                 \
  write_data16(sco);                             \
  write_data16(eco);                             \
  write_cmd(PAGE_ADDRESS_SET);                   \
  write_data16(spg);                             \
  write_data16(epg)

void init_lcd() {
  //Enable extended memory interface with 10 bit addressing
  XMCRB = _BV(XMM2) | _BV(XMM1);
  XMCRA = _BV(SRE);
  //Set reset and backlight pins as outputs
  DDRC |= _BV(RESET);
  DDRB |= _BV(BLC);
  lcd_reset();
  //Disable and wake display whilst we configure it
  write_cmd(DISPLAY_OFF);
  write_cmd(SLEEP_OUT);
  _delay_ms(60);
  write_cmd_data(INTERNAL_IC_SETTING, 0x01);
  //Power / voltage control settings (p.178-180)
  write_cmd(POWER_CONTROL_1);
  write_data16(0x2608);
  write_cmd_data(POWER_CONTROL_2, 0x10);
  write_cmd(VCOM_CONTROL_1);
  write_data16(0x353E);
  write_cmd_data(VCOM_CONTROL_2, 0xB5);
  //Interface control (p.192)
  write_cmd_data(INTERFACE_CONTROL, 0x01);
  write_data16(0x0000);
  //16 bit/pixel (p.134)
  write_cmd_data(PIXEL_FORMAT_SET, 0x55);

  //All this stuff here should be taken out since it really should be implementation specific
  set_orientation(North);
  clear_screen();
  display.x = 0;
  display.y = 0;
  display.background = BLACK;
  display.foreground = WHITE;
  write_cmd(DISPLAY_ON);
  _delay_ms(50);
  write_cmd_data(TEARING_EFFECT_LINE_ON, 0x00);
  EICRB |= _BV(ISC61);
  PORTB |= _BV(BLC);
}

//Reset LCD
void lcd_reset() {
  _delay_ms(1);
  PORTC &= ~_BV(RESET);
  _delay_ms(20);
  PORTC |= _BV(RESET);
  _delay_ms(120);
}

void lcd_brightness(uint8_t i) {
  /* Configure Timer 2 Fast PWM Mode 3 */
  TCCR2A = _BV(COM2A1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  OCR2A = i;
}

//Set LCD orientation
void set_orientation(orientation o) {
  display.orient = o;
  //Configure memory write/read directions (p.127)
  //See LCD datasheet for explanations of parameters
  write_cmd(MEMORY_ACCESS_CONTROL);
  if (o == North) {
    display.width = LCDWIDTH;
    display.height = LCDHEIGHT;
    //MX = 1, BGR = 1
    write_data(0x48);
  } else if (o == West) {
    display.width = LCDHEIGHT;
    display.height = LCDWIDTH;
    //MX,MY,MV = 1, BGR = 1
    write_data(0xE8);
  } else if (o == South) {
    display.width = LCDWIDTH;
    display.height = LCDHEIGHT;
    //MY = 1, BGR = 1
    write_data(0x88);
  } else if (o == East) {
    display.width = LCDHEIGHT;
    display.height = LCDWIDTH;
    //MV = 1, BGR = 1
    write_data(0x28);
  }
  /*
  Column / page address set (p.110-112)
  By default sets RAM write region to 320*240
  When writing to the display you set a RAM region as follows:
  |---------------------------|
  |                           |
  |    [SC]           [EC]    |
  |     *..............*      |
  |     .              .      |
  |     .              .      |
  |     .              .      |
  |     .              .      |
  |     .              .      |
  |     *..............*      |
  |    [SP]           [EP]    |
  |                           |
  |---------------------------|
  Then as you shove out data over the parallel interface,
  that region of RAM will be written to. REMEMBER:
  we use 16 bpp mode so each pixel has 16 bits (R-5,G-6,B-5).
  The code below is defined in the macro set_ram_write_region, but
  it is left here for ease of understanding
  */
  write_cmd(COLUMN_ADDRESS_SET);
  write_data16(0);                  //S(tart) C(olumn)
  write_data16(display.width - 1);  //E(nd) C(olumn)
  write_cmd(PAGE_ADDRESS_SET);
  write_data16(0);                   //S(tart) P(age)
  write_data16(display.height - 1);  //E(nd) P(age)
}

void set_frame_rate_hz(uint8_t framerate) {
  //Various parameters for framerate settings (yet to fully understand)
  uint8_t DIVA;    //Division ratio for clock
  uint8_t RTNA;    //Clocks/line?
  uint8_t period;  //Magic?
  if (framerate > 118)
    framerate = 118;
  if (framerate < 8)
    framerate = 8;
  if (framerate > 60) {
    DIVA = 0x00;
  } else if (framerate > 30) {
    DIVA = 0x01;
  } else if (framerate > 15) {
    DIVA = 0x02;
  } else {
    DIVA = 0x03;
  }
  period = 1920.0 / framerate;
  RTNA = period >> DIVA;
  //Frame rate control (p.155)
  write_cmd(FRAME_CONTROL_IN_NORMAL_MODE);
  write_data(DIVA);
  write_data(RTNA);
}

void fill_rectangle(rectangle r, uint16_t colour) {
  set_ram_write_region(r.left, r.right, r.top, r.bottom);
  //Set controller into memory write mode
  write_cmd(MEMORY_WRITE);
  uint16_t width = r.right - r.left + 1;
  uint16_t height = r.bottom - r.top + 1;

  /*
    Here is some logic to get around the fact that apparently the super smart
    compiler can't handle us multiplying two numbers whose result is over 16 bits.
    Not entirely sure how it works but I don't have the brainpower currently
    to figure it out
  */
  uint16_t mod8, div8;
  uint16_t odm8, odd8;
  if (height > width) {
    mod8 = height % 8;    //Height % 8
    div8 = height >> 3;   //Height / 8
    odm8 = width * mod8;  //Width * (height % 8)
    odd8 = width * div8;  //Width * (height / 8)
  } else {
    mod8 = width % 8;
    div8 = width >> 3;
    odm8 = height * mod8;
    odd8 = height * div8;
  }
  uint8_t pix1 = odm8 % 8;
  while (pix1--)  //Shove data into RAM `pix1` times
    write_data16(colour);
  uint16_t pix8 = odd8 + (odm8 >> 3);
  while (pix8--) {
    write_data16(colour);
    write_data16(colour);
    write_data16(colour);
    write_data16(colour);
    write_data16(colour);
    write_data16(colour);
    write_data16(colour);
    write_data16(colour);
    _delay_ms(1);
  }
}

/*
Draw an image (rect with a colour[] array of the colour of each pixel)
*/
void fill_rectangle_indexed(rectangle r, uint16_t *colour) {
  uint16_t x, y;
  set_ram_write_region(r.left, r.right, r.top, r.bottom);
  write_cmd(MEMORY_WRITE);
  for (x = r.left; x <= r.right; x++)
    for (y = r.top; y <= r.bottom; y++)
      write_data16(*colour++);
}

/*
Clear screen
*/
void clear_screen() {
  //We basically draw a rect with no colour over the entire display
  display.x = 0;
  display.y = 0;
  rectangle r = {0, display.width - 1, 0, display.height - 1};
  fill_rectangle(r, display.background);
}

/*
Display a character
*/
void display_char(char charToWrite) {
  uint16_t x, y;
  PGM_P fdata;
  uint8_t bits, mask;
  //Memory write bounds (same variables as described above)
  uint16_t sco = display.x,
           eco = display.x + 4,
           spg = display.y,
           epg = display.y + 7;

  //New line starts a new line, or if the end of the display has been reached, clears the display.
  if (charToWrite == '\n') {
    //display.x/y are global identifiers of the place on the display where we should write the next character to
    display.x = 0;
    display.y += 8;
    if (display.y >= display.height) {
      clear_screen();
    }
    return;
  }

  //We only want to handle ascii printable characters:
  if (charToWrite < 32 || charToWrite > 126) return;

  /*
    Get the font data for the character to write
    The font starts at ' ' so we must subtract the value of ' ' (32) from
    the character we want to write to get the correct index of the font data
  */
  fdata = ((charToWrite - ' ') * 5) + font5x7;
  //Set page (y value) bounds
  write_cmd(PAGE_ADDRESS_SET);
  write_data16(spg);
  write_data16(epg);

  /*
  For every column in the memory region (there will be 5 since the font is 5*7),
  we set the RAM write column to be that column only (remember the page address
  was set above). Then we grab the font data for that column and write that into display memory
  */
  for (x = sco; x <= eco; x++) {
    write_cmd(COLUMN_ADDRESS_SET);
    write_data16(x);
    write_data16(x);
    write_cmd(MEMORY_WRITE);

    bits = pgm_read_byte(fdata);  //Get the byte of the current column from the font data
    fdata++;
    //We loop over each pixel in the column, going down vertically from the top of the character to the bottom
    for (y = spg, mask = 0x01; y <= epg; y++, mask <<= 1)
      /*
        (bits & mask) will grab the correct bit from the font column byte, then we either draw a pixel if the bit is
        set, else we draw the background colour
      */
      write_data16((bits & mask) ? display.foreground : display.background);
    /*
      For reference: '!' is:
      (0x00, 0x00, 0x5F, 0x00, 0x00), which looks like:
      0 0 1 0 0
      0 0 1 0 0
      0 0 1 0 0
      0 0 1 0 0
      0 0 1 0 0
      0 0 0 0 0
      0 0 1 0 0
      Where each byte represents a column of the character, with 
      the LSB corresponding to the top of the character
    */
  }

  /*
    Finally we set the column address to be the column of pixels
    to the right of the character that we just wrote, and make that
    column the background colour, meaning we can write text over
    any colour and there won't be 1 pixel slivers of that background poking
    through.
  */
  write_cmd(COLUMN_ADDRESS_SET);
  write_data16(x);
  write_data16(x);
  write_cmd(MEMORY_WRITE);
  for (y = spg; y <= epg; y++)
    write_data16(display.background);

  //Move character write pointer along 6 (font width + 1 for spacing)
  display.x += 6;
  //If we reach the edge of the display, move the pointer back to the left edge and down a line
  if (display.x >= display.width) {
    display.x = 0;
    display.y += 8;
  }
}

/*
Display a string (call display_char for each char in the string)
*/
void display_string(char *str) {
  uint8_t i;
  for (i = 0; str[i]; i++)
    display_char(str[i]);
}

/*
Display a string at a given x and y coordinate
*/
void display_string_xy(char *str, uint16_t x, uint16_t y) {
  uint8_t i;
  //Set the character pointer coordinate to the argument values
  display.x = x;
  display.y = y;
  for (i = 0; str[i]; i++)
    display_char(str[i]);
}

/*
Helper function to diplay a byte as ascii, might be useful for debugging
*/
void display_register(uint8_t reg) {
  uint8_t i;

  for (i = 0; i < 8; i++) {
    if ((reg & (_BV(7) >> i)) != 0) {
      display_char('1');
    } else {
      display_char('.');
    }
  }
}
