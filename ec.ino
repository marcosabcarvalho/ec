#include <Arduino.h>
#include "ec_conf.h"
#include <Float64.h>
#include <Math64.h>
#if defined(ARDUINO)
#include <avr/sleep.h>
#endif

#include "softkey.h"
#include "display.h"
#include "rpn.h"

Softkey skey;

display lcd;
//const byte pin_BL=10;

rpn sysrpn;
//Sleep sleep;

const int8_t rpin[7]={KEY_R0, KEY_R1, KEY_R2, KEY_R3, KEY_R4, KEY_R5, KEY_R6};
const int8_t cpin[]={KEY_C0,KEY_C1,KEY_C2,KEY_C3,KEY_C4};

static const char keyval[]="\
abcde\
fghij\
\r\rPE\010\
?789/\
S456*\
R123-\
!0.X+";

static const char keychar[]="\
abcde\
fghij\
.....\
.klmn\
.opqr\
.stuv\
.wxyz";

int sleep_req=0;
void gosleep();

void setup (void)
{
#ifdef TEST_SMALL  
  Serial.begin (115200);
#endif  
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
  int8_t r,c;
  bool keydown=false;
  int8_t key=NO_KEY;

  lcd.setPinMode(INPUT);
  delay(10);
  for(c=0;c<5;c++){
    pinMode(cpin[c],OUTPUT);
    digitalWrite(cpin[c],LOW);
    for(r=0;r<7;r++){
      if(digitalRead(rpin[r]) == LOW){
        keydown=true;
        key=c+r*5;
        goto xit; //return first key found
      }
    }
    pinMode(cpin[c],INPUT); //high Z
  }
xit:
  lcd.setPinMode(OUTPUT);//for lcd
  return key;
}

int8_t getKey(void)
{
  while(scankeys()>=0); //wait for release
  return scankeys(); //return next press
}

void loop(void)
{
  char kv=NO_KEY, kc=0;
  char keypressed = getKey();
  if(keypressed > NO_KEY){
    kv = keyval[keypressed];
    kc = keychar[keypressed];
  }
#ifdef TEST_SMALL
  else{
    kv = kc = skey.getKey();
  }
#endif  
  if (kv != NO_KEY){
    sysrpn.key_input(kv,kc);
  }
  if(sleep_req){
    lcd.command(LCD_DISPLAYCONTROL|LCD_DISPLAYOFF);
    analogWrite(10,0);
    while(!digitalRead(rpin[6]))delay(100); //wait for key release
    gosleep();
    delay(100);
    sysrpn.key_input('!','.'); // complete the wake up
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

void wake(void){}

void gosleep()
{
#if defined(ARDUINO)  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  //attachInterrupt(digitalPinToInterrupt(2),wake,LOW);
  cli();
  ADCSRA &= ~(1<<ADEN);  // adc off
  EICRA=0; //low level on pin 2
  EIMSK=1; //enable INT0
  DIDR0=0x3f; //disable digital input on ADC 0..5
  DIDR1=0x03; //disable digital input on AIN0/1
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  sleep_disable();
  DIDR0 = DIDR1 = 0; //reenable digital input
#endif  
}

#if defined(ARDUINO)  
ISR(INT0_vect)
{
  EIMSK=0; //disable INT0
}
#endif

