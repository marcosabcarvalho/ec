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
  if(altFn&alt_Edit)key_edit(key);
  else key_norm(key);
}

void rpn::key_norm(char key)
{
  f64 undo_lx = lastx;
  lastx = stackx;
  if(key!='A')busy();
  if(altFn&alt_Shift)key_shift(key);
  else switch(key){
    case 'q'://exp
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
      stackx=f64(1)/stackx;
      break;
    case 'b':
      stackx=sqrt64(stackx);
      break;
    case 'c':
      stackx=exp64(stack_pull() * log64(stacky));
      break;
    case 'd':
      stackx=log64(stackx);
      break;
    case 'e':
      stackx=log64(stackx)/log64(10);
      break;
    case 'f':
      stack_swapxy();
      break;
    case 'g':
      stack_pull();
      break;
    case 'h':
      //stackx=altFn&alt_Hyp?sinh64(stackx):sin64(stackx);
      stackx=sin64(stackx);
      altFn&=~alt_Hyp;
      break;
    case 'i':
      //stackx=altFn&alt_Hyp?cosh64(stackx):cos64(stackx);
      stackx=cos64(stackx);
      altFn&=~alt_Hyp;
      break;
    case 'j':
      //stackx=altFn&alt_Hyp?tanh64(stackx):tan64(stackx);
      stackx=tan64(stackx);
      altFn&=~alt_Hyp;
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
      altFn ^= alt_Shift;
      lastx = undo_lx;
      break;
    case 'B':
    case 'C':
    case 'D':
      break;
    case '\r':
    case '\n':
      stack_push();
      break;

    default:
      PrDev->print(key);
      break;
  }
  show_stack();
}

void rpn::key_shift(char key)
{
  f64 undo_lx = lastx;
  lastx = stackx;
  if(key!='A')busy();
  switch(key){
    case 'A': //unshift
      lastx = undo_lx;
      altFn ^= alt_Shift;
      return;
    case 'a':
      stack_push();
      stackx = lastx;
      break;
    case 'b':
      stackx = stackx*stackx;
      break;
    case 'c':
      stackx = exp64((f64(1)/stack_pull()) * log64(stacky));
      break;
    case 'd':
      stackx=exp64(stackx);
      break;
    case 'e':
      stackx=stack_pull()*log64(10);
      break;
    case 'f':
      stack_push();
      stackx=pi;
    case 'g':
      lastx = undo_lx;
      altFn ^= alt_Hyp;
      break;
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
      break;
    case 'p':
      stackx=stackx.fabs();
      break;
    case 'q':
      stackx=stackx.intval();
      break;
    case '/':
      stackx%=stack_pull();
      break;
#ifdef EXTRA_FN
    case 'h':
      stackx=altFn&alt_Hyp?asinh64(stackx):asin64(stackx);
      break;
    case 'i':
      stackx=altFn&alt_Hyp?acosh64(stackx):acos64(stackx);
      break;
    case 'j':
      stackx=altFn&alt_Hyp?atanh64(stackx):atan64(stackx);
      break;
    case '7':
      stackx=stack_pull()==stackx;
      break;
    case '8':
      stackx=stack_pull()<stackx;
      break;
    case '9':
      stackx=stack_pull()<=stackx;
      break;
    case '4':
      //->R
      break;
    case '5':
      //->P
      break;
    case '6':
      //random
      break;
    case '*':
      stackx=stackx==0;
      break;
    case '1':
      //->HMS
      break;
    case '2':
      //HMS->
      break;
    case '3':
      //seed
      break;
    case '-':
      stackx=(stackx<0);
      break;
    case '+':
      stackx=(stackx<=0);
      break;
    case 'B':
      Serial.print(stackx.bits(1),16);
      Serial.println(stackx.bits(0),16);
      Serial.println();
      break;
#else
    case 'h':
      stackx=asin64(stackx);
      break;
    case 'i':
      stackx=acos64(stackx);
      break;
    case 'j':
      stackx=atan64(stackx);
      break;
    case '*':
    case '-':
    case '+':
    case '0' ... '9':
      key_norm(key);
      break;      
#endif      
    default:
      lastx=stackx;
      break;
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

void rpn::busy(void)
{
  PrDev->clear();
  PrDev->lcdprint(" ...",1);
}

