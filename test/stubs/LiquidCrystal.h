#ifndef _liquidcrystal_h_
#define _liquidcrystal_h_
#include <Arduino.h>

class LiquidCrystal : public Print
{
  public:
    LiquidCrystal(){}
    void begin(int,int){}
    void clear(void){printf("\r\n");}
    void home(void){printf("\r\n");}
    void setCursor(int,int){printf("\r\n");}
    void setPinMode(int){}
};

#endif

