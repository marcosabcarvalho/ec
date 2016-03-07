#include "display.h"

/* simple wrapper class to handle line-feed on the LCD */

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

void display::println(const char *str)
{
  print(str);
  setCursor(1,1);
}

void display::println(char ch)
{
  print(ch);
  setCursor(1,1);
}

void display::println(const Printable& p)
{
  print(p);
  setCursor(1,1);
}

void display::displayline(int n)
{
  if(n){
    setCursor(1,0);
    clearline(1);
  }
  else setCursor(1,1);
}

/* 0 is the bottom line */
void display::clearline(int n)
{
  println("               ");
  setCursor(1,!n);
}


