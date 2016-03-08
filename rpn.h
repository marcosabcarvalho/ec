#include <Math64.h>
#include "display.h"

#define STACK_DEPTH 4
#define STACK_TOP (STACK_DEPTH-1)
#define stackx stack[0]
#define stacky stack[1]

typedef enum {alt_Norm, alt_Edit, alt_Shift, alt_Alpha, alt_Hyp} altFn_t;

class rpn
{
  display *PrDev;
  f64 stack[STACK_DEPTH];
  f64 lastx;
  altFn_t altFn;
  bool push_en;
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

};


