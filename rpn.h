#include <Math64.h>
#include "display.h"

#define STACK_DEPTH 4
typedef enum {alt_Norm, alt_Edit, alt_Shift, alt_Alpha, alt_Hyp} altFn_t;

class rpn
{
  display *PrDev;
  f64 stack[STACK_DEPTH];
  altFn_t altFn;
  public:
  void begin(display &dev);
  rpn(void);
  void key_input(char key);

  private:
  void displayline(int n);
  void key_norm(char key);
  void key_edit(char key);
  void stack_push(void);
};


