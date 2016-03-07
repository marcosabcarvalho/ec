/* Arduino.cxx:
 * for PC make only.
 * the cxx extention prevents the IDE from compiling this
 */

#include <Arduino.h>
#include <unistd.h>
#include <Math64.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

Print Serial;
static char ch_;

int main(int argc, char *argv[])
{
  f64 d1;
  int i,k;
  static struct termios oldt, newt;

  tcgetattr( STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);          
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);

  setup();
  printf("\n---=!!!!!!=---\n");
  printf("Running loop()\n");
  printf("Press 'q' to quit\n");
  for(;;){
    loop();
    //if(ch_=='\n')putchar('\n');
    if(ch_=='q')break;
  }

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
  putchar('\n');
  return 0;
}

char Print::read(void)
{
  ch_ = getchar();
  return ch_;
}

