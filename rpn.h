#include <Math64.h>
#include "display.h"

#define STACK_DEPTH 4
#define STACK_TOP (STACK_DEPTH-1)
#define stackx (stack[stack_index])
#define stacky (stack[(stack_index+1)%STACK_DEPTH])
#define stackt (stack[(stack_index+STACK_TOP)%STACK_DEPTH]) //top
#define stacks (stack[(stack_index+STACK_TOP-1)%STACK_DEPTH]) //top-1

#define  alt_Norm 0
#define  alt_Edit 1
#define  alt_Shift 2
#define  alt_Hyp 4
#define  alt_Alpha 8
#define  alt_Sto 16
#define  alt_Rcl 32
#define  alt_Fix 64

class rpn
{
  display *PrDev;
  f64 stack[STACK_DEPTH];
  f64 lastx;
  uint8_t stack_index;
  uint8_t altFn;
  bool push_en;
  f64 stovars[20];
  f64 todeg;
  bool deg_en;

  public:
  void begin(display &dev);
  rpn(void);
  void key_input(char key);
  void show_stack(void);

  private:
  void displayline(int n);
  void key_norm(char key);
  void key_edit(char key);
  void key_shift(char key);
  void stack_push(void);
  f64 stack_pull(void);
  void stack_swapxy(void);
  void busy(void);
  int8_t varidx(char key);
  void sto(char key);
  void rcl(char key);
};


