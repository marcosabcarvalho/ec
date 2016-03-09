#include "rpn.h"

static void cdel(char *buf)
{
  for(;*buf;buf++)
    *(buf)=*(buf+1);
}

static void cins(char *buf, char c)
{
  for(;c;buf++){
    c^=*buf;
    *buf^=c;
    c^=*buf;
  }
  *buf='\0';
}

static void set_exp_sign(char *buf, bool neg)
{
  int8_t n = 0;
  
  while(buf[n] && buf[n]!='e')n++; // string MUST contain an 'e' 
  if(!buf[n])return;
  n++;
  if(neg && buf[n]=='-')return;
  if(!neg && buf[n]!='-')return;
  if(!neg)cdel(&buf[n]); //delete '-'    
  else cins(&buf[n],'-'); //insert '-'
}

void rpn::key_edit(char key)
{
  static char edln[18];
  static int8_t edpos=0,expos=0;
  static bool neg,point,ex, sexp=0;
  static int16_t exv=0;

  if(!(altFn&alt_Edit)){
    altFn |= alt_Edit;
    edln[0]='-'; //it is easier to insert the sign first and remove it later
    for(int8_t i=1;i<18;i++)edln[i]=0;
    PrDev->lcdclear(0);
    edpos=1;
    point=neg=false;
    exv=0;
    sexp=ex=false;
  }
  switch(key){
    case 'B':
      stack_push();
      stackx = strtof64(&edln[!neg], NULL);
      key_norm(key);
      break;
    case '0' ... '9':
      if(ex){
        if(exv*10+(key-'0')>=307)break;
        exv*=10;
        exv+=key-'0';
      }
      edln[edpos++]=key;
      PrDev->print(key);
      break;
    case 'p': //sign
      if(ex){
        sexp^=1;
        set_exp_sign(edln+expos,sexp);
        edpos+=sexp?1:-1;
      }
      else{
        neg^=1; //toggle sign
      }
      PrDev->lcdclear(0);
      PrDev->print(&edln[!neg]);
      break;
    case '.':
      if(!point){
        if(edpos<2){
          PrDev->print('0');
          edln[edpos++]='0';
        }
        edln[edpos++]='.';
        PrDev->print('.');
      }
      point=true;
      break;
    case 'q':
      if(!ex){
        if(edpos==1){ /* eex cannot be first */
          PrDev->print('1');
          edln[edpos++]='1';
        }
        expos=edpos;
        edln[edpos++]='e';
        PrDev->print('e');
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
        PrDev->lcdprint(stackx.toString());
      }
      else{
        PrDev->lcdclear(0);
        PrDev->lcdprint(&edln[!neg]);
      }
      break;
    case '\n':
    case '\r':
      altFn=alt_Norm;
      edln[edpos]='\0';
      stackx = strtof64(&edln[!neg], NULL);
      push_en=false;
      key_norm(key);
      return;
      break;
    case 'A': //shift
      altFn^=alt_Shift;
      break;
    default:
      altFn&=~alt_Edit;
      edln[edpos]='\0';
      stackx = strtof64(&edln[!neg], NULL);
      key_norm(key);
  }
}

