#include "rpn.h"

rpn::rpn(void)
{
  altFn = alt_Norm;
  lastx=0;
  push_en=true;
  deg_en=true;
  stack_index=0;
  //todeg=f64(90)/f64( 0x3FF921FB, 0x54442D18);//pio2 not defined yet!?
}

void rpn::begin(display &dev)
{
  PrDev=&dev;
  todeg=f64(90)/pio2;//this doesn't work in the constructor!?
  show_stack();
}

void rpn::key_input(char key)
{
  if(altFn&alt_Edit)key_edit(key);
  else key_norm(key);
}

void rpn::key_norm(char key)
{
  f64 undo_lx=lastx;
  if(altFn&alt_Shift)key_shift(key);
  else if(altFn&alt_Sto)sto(key);
  else if(altFn&alt_Rcl)rcl(key);
  else{
    if(key!='A' && !(altFn&alt_Sto)){
      busy();
      lastx = stackx;
    }
    switch(key){
    case 'q'://exp
    case '.':
    case '0' ... '9':
      if(push_en)stack_push();
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
      //altFn&=~alt_Hyp;
      //stackx=altFn&alt_Hyp?sinh64(stackx):sin64(stackx/todeg);
      stackx=sin64(stackx/todeg);
      break;
    case 'i':
      //altFn&=~alt_Hyp;
      //stackx=altFn&alt_Hyp?cosh64(stackx):cos64(stackx/todeg);
      stackx=cos64(stackx/todeg);
      break;
    case 'j':
      //altFn&=~alt_Hyp;
      //stackx=altFn&alt_Hyp?tanh64(stackx):tan64(stackx/todeg);
      stackx=tan64(stackx/todeg);
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
      altFn |= alt_Shift;
      lastx = undo_lx;
      break;
    case 'B':
      PrDev->lcdprint("STO:_");
      altFn |= alt_Sto;
      return;
    case 'C':
      PrDev->lcdprint("RCL:_");
      altFn |= alt_Rcl;
      return;
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
  }
  show_stack();
}

void rpn::key_shift(char key)
{
  f64 undo_lx = lastx;;
  if(key!='a' && key!='A' && !(altFn&alt_Sto)){
    busy();
    lastx = stackx;
  }
  switch(key){
    case 'A': //unshift
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
      stackx=f64( 0x3FF921FB, 0x54442D18)*f64(2);
    case 'p':
      stackx=stackx.fabs();
      break;
    case 'q':
      stackx=stackx.intval();
      break;
#ifdef EXTRA_FN
    case 'g':
      lastx = undo_lx;
      altFn ^= alt_Hyp;
      break;
    case '/':
      //stackx%=stack_pull();
      stackx=stacky.ipart()%(long)(stack_pull()); //32-bit version saves ram
      break;
    case 'h':
      stackx=altFn&alt_Hyp?asinh64(stackx):asin64(stackx)*todeg;
      break;
    case 'i':
      stackx=altFn&alt_Hyp?acosh64(stackx):acos64(stackx)*todeg;
      break;
    case 'j':
      stackx=altFn&alt_Hyp?atanh64(stackx):atan64(stackx)*todeg;
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
    case 'g': // deg/rad
      lastx = undo_lx;
      deg_en=!deg_en;
      if(deg_en)todeg=f64(90)/f64( 0x3FF921FB, 0x54442D18);
      else todeg=1;
      break;
    case 'h':
      stackx=asin64(stackx)*todeg; //no hyp functions
      //stackx=altFn&alt_Hyp?asinh64(stackx):asin64(stackx)*todeg;
      break;
    case 'i':
      stackx=acos64(stackx)*todeg;
      //stackx=altFn&alt_Hyp?acosh64(stackx):acos64(stackx)*todeg;
      break;
    case 'j':
      stackx=atan64(stackx)*todeg;
      //stackx=altFn&alt_Hyp?atanh64(stackx):atan64(stackx)*todeg;
      break;
    case '*':
    case '-':
    case '+':
    case '0' ... '9':
      key_norm(key);
      break;      
    case '/':
      stackx=stacky.ipart()%(long)(stack_pull()); //32-bit version saves ram
      break;
/*      
    case 'B':
      Serial.print(stackx.bits(1),16);
      Serial.println(stackx.bits(0),16);
      Serial.println();
      break;
//*/      
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
  int i,j;
  PrDev->lcdprint(stacky.toString(),1);
  PrDev->lcdprint(stackx.toString());
  Serial.println();
  Serial.println("----------------");

  for(i=0,j=stack_index+STACK_TOP;i<STACK_DEPTH;i++,j--){
    Serial.println(stack[j%STACK_DEPTH]);
  }
}

void rpn::stack_push(void)
{
  stack_index+=STACK_TOP;
  stack_index%=STACK_DEPTH;
  stackx = stacky;
}

f64 rpn::stack_pull(void)
{
  stack_index++;
  stack_index%=STACK_DEPTH;
  //stackt = stacks; //duplicate the stack top
  stackt = 0; //zero stack top
  return lastx;
}

void rpn::stack_swapxy(void)
{
  f64 z=stacky;
  stacky=stackx;
  stackx=z;
}

void rpn::busy(void)
{
  PrDev->clear();
  PrDev->lcdprint(" ...",1);
}

int8_t rpn::varidx(char key)
{
  int8_t n=-1;
  if(key>='a' && key<='j')n=key-'a';
  else if(key>='0' and key<='9')n=key-'0'+10;
  return n;
}
void rpn::sto(char key)
{
  int8_t n = varidx(key);
  if(n>=0){
    stovars[n] = stackx;
    PrDev->print(key);
    delay(200);
  }
  PrDev->lcdprint(stackx.toString());
  altFn&=~alt_Sto;
}

void rpn::rcl(char key)
{
  int8_t n = varidx(key);
  if(n>=0){
    stack_push();
    stackx = stovars[n];
  }
  PrDev->lcdprint(stackx.toString());
  altFn&=~alt_Rcl;
}

