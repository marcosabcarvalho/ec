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
\r\r_E\010\
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

int16_t sleeptimer=0;

int sleep_req=0;
void gosleep();

void setup (void)
{
  uint8_t eex_char[]={0,0xf,0x8,0xf,0x8,0xf,0,0};
#if defined(TEST_SMALL) || !defined(ARDUINO) //need serial port for testing
  Serial.begin (115200);
#endif  
  lcd.clear();
  lcd.createChar(EXPCHAR,eex_char);
  lcd.blink();
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

  //lcd.setPinMode(INPUT); //columns go high Z
  //delay(10);
  for(c=0;c<5;c++){
    digitalWrite(cpin[c],HIGH);
  }
  for(c=0;c<5;c++){
    //pinMode(cpin[c],OUTPUT);
    digitalWrite(cpin[c],LOW); //drive columns low one at a time
    for(r=0;r<7;r++){
      if(digitalRead(rpin[r]) == LOW){ //scan rows
        keydown=true;
        key=c+r*5;
        goto xit; //return first key found
      }
    }
    //pinMode(cpin[c],INPUT); //high Z
    digitalWrite(cpin[c],HIGH);
  }
xit:
  //lcd.setPinMode(OUTPUT);//for lcd
  return key;
}

//stop scanning while waiting for key release
void waitRelease(int key)
{
  int c=key%5;
  int r=key/5;
  if(key<0)return;
  for(int i=0;i<5;i++){
    digitalWrite(cpin[i],i==c?LOW:HIGH);
  }
  while(digitalRead(rpin[r])==LOW)delay(10);
}

void loop(void)
{
  char kv=NO_KEY, kc=0;
  int8_t keypressed = scankeys();
  
  if(keypressed > NO_KEY){
    kv = keyval[keypressed];
    kc = keychar[keypressed];
    sleeptimer=0;
  }
#if defined(TEST_SMALL) || !defined(ARDUINO)
  else{
    kv = kc = skey.getKey(); //use serial port if no hardware keyboard is available
  }
#endif  
  if (kv != NO_KEY){
    sysrpn.key_input(kv,kc);
  }
  if(sleep_req || sleeptimer>600){
    lcd.command(LCD_DISPLAYCONTROL|LCD_DISPLAYOFF);
    analogWrite(10,0);
    while(!digitalRead(rpin[6]))delay(100); //wait for key release
    gosleep();
    delay(100);
    sleeptimer=0;
    sysrpn.key_input('!','.'); // complete the wake up
  }
  waitRelease(keypressed);
  sleeptimer++;
  delay(50);
  //while(scankeys()!=NO_KEY); //this doesn't work well
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

#if defined(ARDUINO)  
void gosleep()
{
  for(int i=0;i<5;i++){
    digitalWrite(cpin[i],i?HIGH:LOW);
  }
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
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
}

ISR(INT0_vect)
{
  EIMSK=0; //disable INT0
}
#else
void gosleep(){}
#endif

