#include "ec_conf.h"
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
  
  while(buf[n] && buf[n]!=EXPCHAR)n++; // string MUST contain an EXPCHAR
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
  f64 result;

  if(!(altFn&alt_Edit)){
    altFn |= alt_Edit;
    edln[0]='-'; //it is easier to insert the sign first and remove it later
    for(int8_t i=1;i<18;i++)edln[i]=0;
    if(!(altFn&alt_Fix)){
      PrDev->lcdclear(0);
      show_flags();
    }
    edpos=1;
    point=neg=false;
    exv=0;
    sexp=ex=false;
    PrDev->cursor();
    PrDev->blink();
  }
  switch(key){
    case 0: //dummy: do nothing
      return;
    case 'a' ... 'f': //hex digits
      if(!hex_en)goto def;
      key+='A'-'a'; //substitute capital
      //drop through
    case '0' ... '9':
      if(ex){
        if(exv*10+(key-'0')>=307)break;
        exv*=10;
        exv+=key-'0';
      }
      edln[edpos++]=key;
      PrDev->print(key);
      if( (altFn&alt_Fix) && ((edpos>2) || (key>'1')) ){
        goto xit;
      }
      break;
    case '_': //sign
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
    case 'E':
      if(!ex){
        if(edpos==1){ /* eex cannot be first */
          PrDev->print('1');
          edln[edpos++]='1';
        }
        expos=edpos;
        edln[edpos++]=EXPCHAR;
        PrDev->print((char)EXPCHAR);
      }
      ex=true;
      exv=0;
      sexp=false;
      break;
    case '\177': //pc raw backspace
    case '\010': //terminal backspace
      edpos--;
      if(edln[edpos]=='.')point=false;
      if(edln[edpos]=='e'){
        ex=false;
      }
      if(ex && edln[edpos]=='-')sexp=false;
      exv=exv/10;
      edln[edpos]='\0';
      if(edpos<2){ //exit edit mode
//        altFn=alt_Norm;
//        PrDev->lcdprint(stackx.toString());
        stack_pull(); //okay, but destroys stack top
        goto out;
      }
      else{
        PrDev->lcdclear(0);
        PrDev->lcdprint(&edln[!neg]);
      }
      break;
    case 'X': //toggle hex mode
      hex_en ^= 1;
      lastx.setBase(hex_en?16:10);
      show_flags();
      PrDev->lcdprint(&edln[!neg]);
      break;
    case CR:
xit:
      edln[edpos]='\0';
      result = strtof64(&edln[!neg], NULL);
      if(altFn&alt_Fix){
        lastx.setDecs(result.ipart());
      }
      else stackx=result;
out:      
      altFn=alt_Norm;
      push_en=true;
      show_stack();
      return;
      break;
    case '?': //shift
      altFn^=alt_Shift;
      show_flags();
      PrDev->setCursor(neg+edpos,1);
      break;
    case '!':
      stack_pull();
      goto out;
      break;
    default: /* do other functions immediately */
def:    
      altFn&=~alt_Edit;
      edln[edpos]='\0';
      stackx = strtof64(&edln[!neg], NULL);
      key_input(key,'.');
      break;
  }
}

