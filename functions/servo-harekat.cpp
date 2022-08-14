#include <Servo.h>  
//motor haye jelo R/L
const byte pmrj=3;
const byte pmlj=5;
//motor haye bala R/L
const byte pmru=6;
const byte pmlu=9;
//motor aghab
const byte pma=10;
//motor state and speed
byte smrj=0;
byte tmrj=0;
byte smlj=0;
byte tmlj=0;
byte smru=0;
byte tmru=0;
byte smlu=0;
byte tmlu=0;
byte sma=0;
byte tma=0;
Servo mtrj,mtlj,mtlu,mtru,mta;
void setup(){
Serial.begin(9600);
mtrj.attach(pmrj);
mtlj.attach(pmlj);
mtru.attach(pmru);
mtlu.attach(pmlu);
mta.attach(pma);
//*************************
mtrj.writeMicroseconds(1500);
mtlj.writeMicroseconds(1500);
mtru.writeMicroseconds(1500);
mtlu.writeMicroseconds(1500);
mta.writeMicroseconds(1500);
delay(7000);
 }
void loop(){
  char jht;int srt;char blp;int sblp;//blp is bala & paieen and sblp is sorat and srt is sorat & jht is jahat
  if(Serial.available()>0){
  String input = Serial.readStringUntil('>');
jht = input[0];
blp=input[3];
  char rc[2]={input[1],input[2]};
 srt=atoi(rc);
   char ru[2]={input[4],input[5]};
 sblp=atoi(ru);
   delay(5);
    Serial.read();
    jrl(jht,srt);ud(blp,sblp);
    }

}
void jrl(char j,byte ri){
switch(j){
  case 'w':mrj(ri,1);mlj(ri,1);break;
  case 's':mrj(ri,2);mlj(ri,2);break;
  case 'd':mrj(ri,2);mlj(ri,1);break;
  case 'a':mrj(ri,1);mlj(ri,2);break;
//  *******************************
  case 'q':mlj(ri/2,1);mrj(ri,1);break;
  case 'e':mlj(ri,1);mrj(ri/2,1);break;
  case 'z':mrj(ri,2);mlj(ri/2,2);break;
  case 'c':mlj(ri,2);mrj(ri/2,2);break;
//  **************************************
  case 'x':mrj(0,0);mlj(0,0);break;
  default:break;
}
  }
//  ***********************************
void ud(char ju,byte ru){
  switch(ju){
    case 'u':mru(ru,1);mlu(ru,1);ma(ru,1);break;
    case 'j':mru(ru,2);mlu(ru,2);ma(ru,2);break;
    case 'i':mru(0,0);mlu(0,0);ma(0,0);break;
    default:break;
  }
  
}
//*************************************motor R jolo
void mrj(byte news,byte newt){
  if(news==0||newt==0){
    mtrj.writeMicroseconds(0);
    Serial.println(0);
tmrj=0;smrj=0;

  }
  if(newt==1){
    mtrj.writeMicroseconds(map(news,1,99,1540,2000));
    Serial.println(map(news,1,99,1540,2000));
smrj=news;tmrj=1;
  }
    if(newt==2){
    mtrj.writeMicroseconds(map(news,1,99,1460,1000));
    Serial.println(map(news,1,99,1460,1000));
smrj=news;tmrj=2;
}

}
//*************************************motor L jolo
void mlj(byte news,byte newt){
 if(news==0||newt==0){
    mtlj.writeMicroseconds(0);
tmlj=0;smlj=0;

  }
  if(newt==1){
    mtlj.writeMicroseconds(map(news,1,99,1540,2000));
smlj=news;tmlj=1;
  }
    if(newt==2){
    mtlj.writeMicroseconds(map(news,1,99,1460,1000));
smlj=news;tmlj=2;
}
}
//************************************motor R bala
void mru(byte news,byte newt){
 if(news==0||newt==0){
    mtru.writeMicroseconds(0);
tmru=0;smru=0;

  }
  if(newt==1){
    mtru.writeMicroseconds(map(news,1,99,1540,2000));
smru=news;tmru=1;
  }
    if(newt==2){
    mtru.writeMicroseconds(map(news,1,99,1460,1000));
smru=news;tmru=2;
}
}
//*****************************************motor L bala
void mlu(byte news,byte newt){
if(news==0||newt==0){
    mtlu.writeMicroseconds(0);
tmlu=0;smlu=0;

  }
  if(newt==1){
    mtlu.writeMicroseconds(map(news,1,99,1540,2000));
smlu=news;tmlu=1;
  }
    if(newt==2){
    mtlu.writeMicroseconds(map(news,1,99,1460,1000));
smlu=news;tmlu=2;
}
}
//*******************************************motor aghab
void ma(byte news,byte newt){
if(news==0||newt==0){
    mta.writeMicroseconds(0);
tma=0;sma=0;

  }
  if(newt==1){
    mta.writeMicroseconds(map(news,1,99,1540,2000));
sma=news;tma=1;
  }
    if(newt==2){
    mta.writeMicroseconds(map(news,1,99,1460,1000));
sma=news;tma=2;
}
}
