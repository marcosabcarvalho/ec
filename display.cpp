#include "display.h"

/* simple wrapper class to handle LCD lines */

display::display(void) : LiquidCrystal(8,9,4,5,6,7)
{
  init();
}

display::display(uint8_t rs,uint8_t en,uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3) : LiquidCrystal(rs,en,d0,d1,d2,d3)
{
  init();
}

void display::init(void)
{
  LiquidCrystal::begin(16,2);
}

void display::lcdprint(const char *str)
{
  lcdprint(str,false);
}

void display::lcdprint(const char *str, bool topline)
{
  lcdclear(topline);
  print(str);
}

void display::lcdclear(bool topline)
{
  setCursor(LCD_LEFT,!topline);
  print("                ");
  setCursor(LCD_LEFT,!topline);
}


