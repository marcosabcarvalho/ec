#include "rpn.h"

rpn::rpn(void)
{
  altFn = alt_Norm;
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
      break;
    case alt_Alpha:
      break;
    case alt_Hyp:
      break;
  }
}

static void set_exp_sign(char *buf, bool neg)
{
  int8_t n = 0;
  char cha, chb;
  
  while(buf[n] && buf[n]!='e')n++; // string MUST contain an 'e' 
  if(!buf[n])return;
  n++;
  if(neg && buf[n]=='-')return;
  if(!neg && buf[n]!='-')return;
  if(!neg){ //delete '-'
    while(buf[n]){
      buf[n]=buf[n+1];
      n++;
    }
  }
  else{
    cha=buf[n];
    buf[n]='-'; //insert '-'
    n++;
    while(buf[n]){
      chb=buf[n];
      buf[n]=cha;
      cha=chb;
      n++;
    }
    buf[n++]=cha;
    buf[n]=0;
  }
}

void rpn::key_edit(char key)
{
  static char edln[18];
  static int8_t edpos=0,expos=0;
  static bool neg,point,ex, sexp=0;
  static int16_t exv=0;

  if(altFn != alt_Edit){
    altFn = alt_Edit;
    edln[0]='-'; //it is easier to insert the sign first and remove it later
    for(int8_t i=1;i<18;i++)edln[i]=0;
    PrDev->displayline(0);
    PrDev->clearline(0);
    edpos=1;
    point=neg=false;
    exv=0;
    sexp=ex=false;
  }
  switch(key){
    case '0' ... '9':
      if(ex){
        if(exv*10+(key-'0')>=307)break;
        exv*=10;
        exv+=key-'0';
      }
      edln[edpos++]=key;
      PrDev->print(key);
      break;
    case '-':
      if(ex){
        sexp^=1;
        set_exp_sign(edln+expos,sexp);
        edpos+=sexp?1:-1;
      }
      else{
        neg^=1; //toggle sign
      }
      PrDev->displayline(0);
      PrDev->clearline(0);
      PrDev->print(&edln[!neg]);
      break;
    case '.':
      if(!point){
        if(edpos<2){
          PrDev->print('0');
          edln[edpos++]='0';
        }
        edln[edpos++]=key;
        PrDev->print(key);
      }
      point=true;
      break;
    case 'e':
      if(!ex){
        expos=edpos;
        edln[edpos++]=key;
        PrDev->print(key);
      }
      ex=true;
      exv=0;
      sexp=false;
      break;
    case '\177': //pc raw backspace
    case '\010': //backspace
      edpos--;
      if(edln[edpos]=='.')point=false;
      if(edln[edpos]=='e'){
        ex=false;
      }
      if(ex && edln[edpos]=='-')sexp=false;
      exv=exv/10;
      edln[edpos]='\0';
      if(edpos<2){ //exit edit mode
        altFn=alt_Norm;
        PrDev->displayline(0);
        PrDev->print(stack[0]);
      }
      else{
        PrDev->displayline(0);
        PrDev->clearline(0);
        PrDev->print(&edln[!neg]);
      }
      break;
    case '\n':
    case '\r':
      altFn=alt_Norm;
      edln[edpos]='\0';
      stack[0] = strtof64(&edln[!neg], NULL);
      key_norm(key);
      break;
    default:
      altFn=alt_Norm;
      edln[edpos]='\0';
      stack[0] = strtof64(&edln[!neg], NULL);
      key_norm(key);
  }
}

void rpn::key_norm(char key)
{
  switch(key){
    case '-':
    case '.':
    case '0' ... '9':
      PrDev->clear();
      PrDev->print(stack[0]);
      stack_push();
      key_edit(key);
      break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
      PrDev->print(key);
      break;

    case '\r':
    case '\n':
      stack_push();
      PrDev->displayline(1);
      PrDev->println(stack[1]);
      PrDev->displayline(0);
      PrDev->println(stack[0]);
      break;

    default:
      PrDev->print((int)key);
      break;
  }
}

void rpn::stack_push(void)
{
  for(int8_t i=STACK_DEPTH-1;i>0;i--){
    stack[i] = stack[i-1];
  }
}

