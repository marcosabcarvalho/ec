#include <HardwareSerial.h>

#define NO_KEY -1

class Softkey
{
  public:
  char getKey(void)
  {
    char key=NO_KEY;
    if(Serial.available() > 0){
      key = Serial.read();
    }
    return key;
  }  
};


