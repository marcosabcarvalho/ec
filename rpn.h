#include <Math64.h>
#include "display.h"

#define STACK_DEPTH 4
#define STACK_TOP (STACK_DEPTH-1)
#define stackx (stack[stack_index])
#define stacky (stack[(stack_index+1)%STACK_DEPTH])
#define stackt (stack[(stack_index+STACK_TOP)%STACK_DEPTH]) //top
#define stacks (stack[(stack_index+STACK_TOP-1)%STACK_DEPTH]) //top-1

#define  alt_Norm 0
#define  alt_Edit (1<<0)
#define  alt_Shift (1<<1)
#define  alt_noPush (1<<2)
#define  alt_Sto (1<<3)
#define  alt_Rcl (1<<4)
#define  alt_Fix (1<<5)
#define  alt_Bri (1<<6)

#if !defined(ARDUINO)
#define CR '\n'
#else
#define CR '\r'
#endif

class rpn
{
  display *PrDev;
  int bri;
  f64 stack[STACK_DEPTH];
  f64 lastx;
  uint8_t stack_index;
  uint8_t altFn;
  //bool push_en;
  f64 stovars[26];
  f64 todeg;
  bool deg_en;
  bool hex_en;
  bool sci_en;
  bool eng_en;
  bool power_en;

  public:
  void begin(display &dev);
  rpn(void);
  void key_input(char key, char ch);
  void stack_push(void);
  void set_x(f64 x){stackx = x;}
  void show_stack(void);
  void show_flags(void);

  private:
  void displayline(int n);
  void key_norm(char key);
  void key_edit(char key);
  void key_shift(char key);
  f64 stack_pull(void);
  void stack_swapxy(void);
  void busy(void);
  int8_t varidx(char key);
  void sto(char key);
  void rcl(char key);
};


