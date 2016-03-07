
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
  {'k', 'l', 'm', 'n', 'o'},
  {'\n', '\n', 'p', 'q', 'r'},
  {'A', '7', '8', '9', '/'},
  {'B', '4', '5', '6', '*'},
  {'C', '1', '2', '3', '-'},
  {'D', '0', '.', '?', '+'},
};

byte rPins[Rows]={0,1,2,3,4,5,6,7}; //inputs
byte cPins[Cols]={4,5,6,7,8}; //outputs (can share with LCD)

Keypad kpd= Keypad(makeKeymap(keymap), rPins, cPins, Rows, Cols);
#else
Softkey kpd;
#endif

display lcd;
const byte pin_BL=10;

rpn sysrpn;

void setup (void)
{
  char str[10];
  f64 d1, d2;
  int i;
  long k;

  Serial.begin (115200);
  sysrpn.begin(lcd);

  pinMode(pin_BL, OUTPUT);
  digitalWrite(pin_BL, HIGH);
  
  lcd.clear();
  lcd.print("Hello world!");

  d1 = atof64("1.2");
  Serial.println(d1);

  Serial.print("Math64::pi = ");
  Serial.println(pi);
  Serial.print("rootThree = ");
  Serial.println(sqrt64(3.0));
  Serial.print("sin: ");
  Serial.println(sin64(d1));
  Serial.print("atan: ");
  Serial.println(atan64(0.33));
  Serial.print("log: ");
  Serial.println(log64(6.0));
  Serial.print("exp: ");
  Serial.println(exp64(5.2));
  Serial.print("sqrt: ");
  Serial.println(sqrt64(2.0));
  d1=atof64("-12.34__56_7_89");
  Serial.println(d1);
  Serial.println(atof64("1"));
  Serial.println(atof64("2"));
  Serial.println(atof64("4"));
  Serial.println(atof64("151"));
  Serial.println(atof64("152"));
  Serial.println(atof64("153"));
  Serial.println(atof64("1e42"));
  d1 = f64(0x48A6F578, 0xC4E0A060);
  Serial.println(d1);
} // end of setup

void loop(void)
{
//*
  char keypressed = kpd.getKey();
  if (keypressed != NO_KEY){
    //Serial.println(keypressed);
    sysrpn.key_input(keypressed);
  }
//*/  
}

int freeRam () {
#ifndef NOT_ARDUINO
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#endif
  return -1;
}


