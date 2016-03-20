#ifndef _liquidcrystal_h_
#define _liquidcrystal_h_
#include <Arduino.h>

#define LCD_DISPLAYCONTROL 0
#define LCD_DISPLAYON 0
#define LCD_DISPLAYOFF 0

class LiquidCrystal : public Print
{
  public:
    LiquidCrystal(){}
    void begin(int,int){}
    void clear(void){printf("\r\n");}
    void home(void){printf("\r\n");}
    void setCursor(int,int){printf("\r\n");}
    void setPinMode(int){}
    void command(int){}
    void createChar(uint8_t, uint8_t*){}
};

#endif

