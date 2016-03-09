#include <Math64.h>
#include "display.h"

#define STACK_DEPTH 4
#define STACK_TOP (STACK_DEPTH-1)
#define stackx stack[0]
#define stacky stack[1]

#define  alt_Norm 0
#define  alt_Edit 1
#define  alt_Shift 2
#define  alt_Hyp 4
#define  alt_Alpha 8
#define  alt_Sto 16
#define  alt_Rcl 32

class rpn
{
  display *PrDev;
  f64 stack[STACK_DEPTH];
  f64 lastx;
  uint8_t altFn;
  bool push_en;
  f64 stovars[20];

  public:
  void begin(display &dev);
  rpn(void);
  void key_input(char key);

  private:
  void displayline(int n);
  void key_norm(char key);
  void key_edit(char key);
  void key_shift(char key);
  void stack_push(void);
  f64 stack_pull(void);
  void stack_swapxy(void);
  void show_stack(void);
  void busy(void);
  int8_t varidx(char key);
  void sto(char key);
  void rcl(char key);
};


