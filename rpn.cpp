#include "rpn.h"

rpn::rpn(void)
{
  altFn = alt_Norm;
  lastx=0;
  push_en=true;
  deg_en=true;
  hex_en=false;
  stack_index=0;
  sci_en = false;
}

void rpn::begin(display &dev)
{
  PrDev=&dev;
  todeg=f64(90)/pio2;//this doesn't work in the constructor!
  show_stack();
}

void rpn::key_input(char key,char ch)
{
  if(altFn&alt_Bri){
    altFn = alt_Norm;
    analogWrite(pin_BL, ((key-'0')%10*16+8));
    show_stack();
  }
  else if(altFn&(alt_Sto|alt_Rcl)){
    if(key!='.'){
      if(altFn&(alt_Sto))sto(ch);
      else rcl(ch);
    }
    altFn&=~(alt_Sto|alt_Rcl);
    show_stack();
  }
  else if((altFn&alt_Edit))key_edit(key);
  else if(altFn&alt_Shift)key_shift(key);
  else key_norm(key);
}

void rpn::key_norm(char key)
{
  f64 undo_lx=lastx;
    if(key!='?' && !(altFn&alt_Sto)){
      busy();
      lastx = stackx;
    }
    switch(key){
    case 'E'://exp
    case '.':
    case '0' ... '9':
      if(push_en && !(altFn&alt_Fix))stack_push(); //some things disable edit push
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
      stack_pull();
      break;
    case 'g':
      stack_swapxy();
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
    case '_':
      stackx=-stackx;
      break;
    case '\177': //pc raw backspace
    case '\010': //backspace
    case '\\':
      stackx = 0;
      push_en=false;
      break;
    case '?':
      altFn ^= alt_Shift;
      lastx = undo_lx;
      break;
    case 'S':
      PrDev->lcdprint("STO");
      altFn |= alt_Sto;
      return;
    case 'R':
      PrDev->lcdprint("RCL");
      altFn |= alt_Rcl;
      return;
    case '!':
      break;
    case CR:
      stack_push();
      break;
    case 'X':
      lastx = undo_lx;
      hex_en ^= 1;
      lastx.setBase(hex_en?16:10);
      break;
    default:
      break;
    }
    show_stack();
}

void rpn::key_shift(char key)
{
  f64 undo_lx = lastx;;
  if(key!='a' && key!='?' && !(altFn&alt_Sto)){
    busy();
    lastx = stackx;
  }
  switch(key){
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
    case '_':
      stackx=stackx.fabs();
      break;
    case 'E':
      stackx=stackx.intval();
      break;
    case '+':
      PrDev->lcdprint("Fix:");
      altFn = alt_Fix;
      key_edit(0);
      return;
    case '!':
      PrDev->lcdprint("BRI:");
      altFn |= alt_Bri;
      return;
      
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
#ifndef TEST_SMALL      
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
#endif      
    case '*': //ENG
    case '-': //SCI
      sci_en ^= 1;
      //lastx.setExpMax(sci_en?1:10);
      break;
    case CR:
    case '0' ... '9':
    case '?':
      key_norm(key);
      return;
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

void rpn::show_flags(void)
{
  PrDev->home();
  PrDev->print(hex_en?'x':deg_en?' ':'r');
  PrDev->setCursor(0,1);
  PrDev->print(altFn&alt_Shift?'^':' ');
}

void rpn::show_stack(void)
{
  int i,j;
  if(altFn&alt_Edit)return;
  show_flags();
  PrDev->lcdprint(stacky.toString(),1);
  PrDev->lcdprint(stackx.toString());
#ifdef NOT_ARDUINO  
  Serial.println();
  Serial.println("----------------");
  for(i=0,j=stack_index+STACK_TOP;i<STACK_DEPTH;i++,j--){
    Serial.println(stack[j%STACK_DEPTH]);
  }
#endif  
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

void rpn::sto(char key)
{
  stovars[key-'a'] = stackx;
}

void rpn::rcl(char key)
{
  if(push_en)stack_push();
  push_en=true;
  stackx = stovars[key-'a'];
}

