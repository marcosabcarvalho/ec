
#include <Arduino.h>
#include <Float64.h>
#include <Math64.h>
#include <Keypad.h>
#include <Key.h>

#include "softkey.h"
#include "display.h"
#include "rpn.h"

#if 0
const byte Rows=8;
const byte Cols=5;

char keymap[Rows][Cols]={
  {'a', 'b', 'c', 'd', 'e'},
  {'f', 'g', 'h', 'i', 'j'},
  {'k', 'l', 'm', ';', '\''},
  {'\n', '\n', 'p', 'q', '\\'},
  {'A', '7', '8', '9', '/'},
  {'B', '4', '5', '6', '*'},
  {'C', '1', '2', '3', '-'},
  {'D', '0', '.', '?', '+'},
};

byte rPins[Rows]={0,1,2,3,4,5,6,7}; //inputs
byte cPins[Cols]={4,5,6,7,8}; //outputs (can share with LCD)

Keypad kpd= Keypad(makeKeymap(keymap), rPins, cPins, Rows, Cols);
#endif
Softkey skey;

display lcd;
const byte pin_BL=10;

rpn sysrpn;

void setup (void)
{
  Serial.begin (115200);
  sysrpn.begin(lcd);

  pinMode(pin_BL, OUTPUT);
  digitalWrite(pin_BL, HIGH);
  
  lcd.clear();
  lcd.print("Hello world!");
/*
  Serial.print("Math64::pi = ");
  Serial.println(pi);
  Serial.print("rootThree = ");
  Serial.println(sqrt64(3.0));
  Serial.print("sin: ");
  Serial.println(sin64(atof64("1.2")));
  Serial.print("atan: ");
  Serial.println(atan64(0.33));
  Serial.print("log: ");
  Serial.println(log64(6.0));
  Serial.print("exp: ");
  Serial.println(exp64(5.2));
  Serial.print("sqrt: ");
  Serial.println(sqrt64(2.0));

  for(int i=0;i<16;i++){
    f64 d1 = pi;
    d1.setDecs(i);
    Serial.println(pi);
  }
  Serial.println(atof64("1e42"));
  Serial.println(atof64("-5.7e4"));
  Serial.println(atof64("5.7e-4"));
  Serial.println(atof64("5.7e48"));
  Serial.println(atof64("5.7e-48"));
//*/
} // end of setup

void loop(void)
{
  char keypressed = NO_KEY;;
  //keypressed = kpd.getKey();
  if (keypressed == NO_KEY){
    keypressed = skey.getKey();
  }
  if (keypressed != NO_KEY){
    //Serial.println(keypressed);
    sysrpn.key_input(keypressed);
  }
}

int freeRam () {
#ifndef NOT_ARDUINO
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#endif
  return -1;
}


