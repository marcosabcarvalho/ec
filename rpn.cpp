#include "rpn.h"

rpn::rpn(void)
{
  altFn = alt_Norm;
  lastx=0;
  push_en=true;
}


void rpn::begin(display &dev)
{
  PrDev=&dev;
}

void rpn::key_input(char key)
{
  switch(altFn){
    case alt_Norm:
      key_norm(key);
      break;
    case alt_Edit:
      key_edit(key);
      break;  
    case alt_Shift:
      key_shift(key);
      break;
    case alt_Alpha:
      break;
    case alt_Hyp:
      break;
  }
}

void rpn::key_norm(char key)
{
  switch(key){
    case 'q'://exp
    case '.':
    case '0' ... '9':
      stack_push();
      PrDev->lcdprint(stacky.toString(),1);
      PrDev->lcdclear(0);
      key_edit(key);
      return;
    case '+':
      stackx+=stack_pull();
      break;
    case '-':
      stackx-=stack_pull();
      break;
    case '*':
      stackx*=stack_pull();
      break;
    case '/':
      stackx/=stack_pull();
      break;
    case 'a':
      lastx=stackx;
      stackx=f64(1)/stackx;
      break;
    case 'b':
      lastx=stackx;
      stackx=sqrt64(stackx);
      break;
    case 'c':
      stackx=exp64(stack_pull() * log64(stacky));
      break;
    case 'd':
      lastx=stackx;
      stackx=log64(stackx);
      break;
    case 'e':
      lastx=stackx;
      stackx=log64(stackx)/log64(10);
      break;
    case 'f':
      stack_swapxy();
      break;
    case 'g':
      stack_pull();
      break;
    case 'h':
      lastx=stackx;
      stackx=sin64(stackx);
      break;
    case 'i':
      lastx=stackx;
      stackx=cos64(stackx);
      break;
    case 'j':
      lastx=stackx;
      stackx=tan64(stackx);
      break;
    case 'k':
    case 'l':
    case 'm':
    case ';':
    case '\'':
      break;
    case 'p':
      stackx=-stackx;
      break;
    case '\177': //pc raw backspace
    case '\010': //backspace
    case '\\':
      stackx = 0;
      push_en=false;
      break;
    case 'A':
      altFn = alt_Shift;
      return;
    case 'B':
    case 'C':
    case 'D':
      break;
    case '\r':
    case '\n':
      stack_push();
      break;

    default:
      PrDev->print((int)key);
      break;
  }
  show_stack();
}

void rpn::key_shift(char key)
{
  switch(key){
    case 'A': //unshift
      altFn = alt_Norm;
      return;
    case 'a':
      stack_push();
      stackx = lastx;
      break;
    case 'b':
      lastx=stackx;
      stackx = stackx*stackx;
      break;
    case 'c':
      stackx = exp64((f64(1)/stack_pull()) * log64(stacky));
      break;
    case 'd':
      lastx=stackx;
      stackx=exp64(stackx);
      break;
    case 'e':
      lastx=stackx;
      stackx=stack_pull()*log64(10);
      break;
    case 'f':
      stack_push();
      stackx=pi;
    case 'g':
      //hyp
      break;
    case 'h':
      lastx=stackx;
      stackx=asin64(stackx);
      break;
    case 'i':
      lastx=stackx;
      stackx=acos64(stackx);
      break;
    case 'j':
      lastx=stackx;
      stackx=atan64(stackx);
      break;
#if 0
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
      break;
    case 'p':
      lastx=stackx;
      stackx=stackx.fabs();
      break;
    case 'q':
      lastx=stackx;
      stackx=stackx.intval();
      break;
    case '7':
      lastx=stackx;
      stackx=stackx==0;
      break;
    case '8':
      lastx=stackx;
      stackx=stackx==stack_pull();
      break;
    case '9':
      lastx=stackx;
      stackx=stackx!=stack_pull();
      break;
    case '/':
      lastx=stackx;
      stackx%=stack_pull();
      break;
    case '4':
      lastx=stackx;
      //->R
      break;
    case '5':
      lastx=stackx;
      //->P
      break;
    case '6':
      //random
      break;
    case '*':
      lastx=stackx;
      stackx=stackx<stack_pull();
      break;
    case '1':
      lastx=stackx;
      //->HMS
      break;
    case '2':
      lastx=stackx;
      //HMS->
      break;
    case '3':
      //seed
      break;
    case '-':
      lastx=stackx;
      stackx=stackx>stack_pull();
      break;
    case 'B':
      Serial.print(stackx.bits(1),16);
      Serial.println(stackx.bits(0),16);
      Serial.println();
      break;
#endif
  }
  altFn = alt_Norm;
  show_stack();
}

void rpn::show_stack(void)
{
  PrDev->lcdprint(stacky.toString(),1);
  PrDev->lcdprint(stackx.toString());
  Serial.println();
  Serial.println("----------------");
  for(int i=STACK_TOP;i>=0;i--)Serial.println(stack[i]);
}

void rpn::stack_push(void)
{
  if(push_en){
    for(int8_t i=STACK_TOP;i>0;i--){
      stack[i] = stack[i-1];
    }
  }
  push_en=true;
}

f64 rpn::stack_pull(void)
{
  lastx = stack[0];
  for(int8_t i=0;i<STACK_TOP;i++){
    stack[i] = stack[i+1];
  }
  stack[STACK_TOP]=0;
  return lastx;
}

void rpn::stack_swapxy(void)
{
  f64 z=stack[1];
  stack[1]=stack[0];
  stack[0]=z;
}

