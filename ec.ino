
#include <Arduino.h>
#include <Float64.h>
#include <Math64.h>

#include "softkey.h"
#include "display.h"
#include "rpn.h"

Softkey skey;

display lcd;
//const byte pin_BL=10;

rpn sysrpn;

const int8_t rpin[7]={2,3,A0,A1,A2,A3,A4};

char keyval[]="\
abcde\
fghij\
\n\nPE\010\
?789/\
S456*\
R123-\
!0.X+";

char keychar[]="\
abcde\
fghij\
.....\
.klmn\
.opqr\
.stuv\
.wxyz";

void setup (void)
{
  Serial.begin (115200);
  lcd.clear();
  sysrpn.begin(lcd);

  pinMode(pin_BL, OUTPUT);
  //digitalWrite(pin_BL, HIGH);
  analogWrite(pin_BL, 100);

  for(int i=0;i<7;i++){
    pinMode(rpin[i], INPUT_PULLUP);
  }
  
/*
  lcd.print("Hello world!");
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

int8_t scankeys(void)
{
  const int8_t cpin[]={4,5,6,7,8}; //shared with lcd
  int8_t i,r,c;
  bool keydown=false;
  int8_t key=NO_KEY;

  lcd.setPinMode(INPUT);
  for(c=0;c<5;c++){
    pinMode(cpin[c],OUTPUT);
    digitalWrite(cpin[c],LOW);
    for(r=0;r<7;r++){
      if(digitalRead(rpin[r]) == LOW){
        keydown=true;
        key=c*5+r;
        break;
      }
    }
    pinMode(cpin[c],INPUT); //high Z
  }
  lcd.setPinMode(OUTPUT);//for lcd
  return key;
}

int8_t getKey(void)
{
  while(scankeys()>=0)delay(10); //wait for release
  return scankeys();
}

void loop(void)
{
  char kv=NO_KEY, kc=0;
  char keypressed = getKey();
  if(keypressed > NO_KEY){
    kv = keyval[keypressed];
    kc = keychar[keypressed];
  }
  else{
    kv = kc = skey.getKey();
  }
  if (kv != NO_KEY){
    //Serial.println(keypressed);
    sysrpn.key_input(kv,kc);
  }
}

/*
int freeRam () {
#ifndef NOT_ARDUINO
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#endif
  return -1;
}
*/


