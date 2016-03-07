#ifndef _display_h_
#define _display_h_

#include <LiquidCrystal.h>
#define LCD_LEFT 1

class display : public LiquidCrystal
{
  int8_t lineno;
  public:
  display(void);
  display(uint8_t rs,uint8_t en,uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3);
  void lcdprint(const char *str);
  void lcdprint(const char *str, bool topline);
/*
  void println(const char *str);
  void println(char ch);
  void println(const Printable& p);
*/
  void lcdclear(bool);

  private:
  void init(void);
};

#endif

