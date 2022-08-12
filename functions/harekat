//motor haye jelo R/L
const byte pamrj=3;
const byte pamlj=4;
const byte pr1mrj=22;
const byte pr2mrj=23;
const byte pr1mlj=26;
const byte pr2mlj=27;
//motor haye bala R/L
const byte pamru=5;
const byte pamlu=6;
const byte pr1mru=30;
const byte pr2mru=31;
const byte pr1mlu=28;
const byte pr2mlu=29;
//motor aghab
const byte pr1ma=7;
const byte pr2ma=8;
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
void setup(){
  pinMode(pamrj,OUTPUT);
  pinMode(pamlj,OUTPUT);
  pinMode(pr1mrj,OUTPUT);
  pinMode(pr2mrj,OUTPUT);
  pinMode(pr1mlj,OUTPUT);
  pinMode(pr2mlj,OUTPUT);
  pinMode(pamru,OUTPUT);
  pinMode(pamlu,OUTPUT);
  pinMode(pr1mru,OUTPUT);
  pinMode(pr2mru,OUTPUT);
  pinMode(pr1mlu,OUTPUT);
  pinMode(pr2mlu,OUTPUT);
  pinMode(pr1ma,OUTPUT);
 pinMode(pr2ma,OUTPUT);
 Serial.begin(9600);
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
 if(srt!=0){
srt=map(srt,1,99,110,255);}
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
digitalWrite(pr1mrj,LOW);
digitalWrite(pr2mrj,LOW);
analogWrite(pamrj,0);
tmrj=0;smrj=0;

  }
  if(newt==1){
digitalWrite(pr1mrj,LOW);
digitalWrite(pr2mrj,HIGH);
analogWrite(pamrj,news);
smrj=news;tmrj=1;
  }
    if(newt==2){
digitalWrite(pr2mrj,LOW);
digitalWrite(pr1mrj,HIGH);
analogWrite(pamrj,news);
smrj=news;tmrj=2;
}

}
//*************************************motor L jolo
void mlj(byte news,byte newt){
  if(news==0||newt==0){
digitalWrite(pr1mlj,LOW);
digitalWrite(pr2mlj,LOW);
analogWrite(pamlj,0);
tmlj=0;smlj=0;
  }
  if(newt==1){
digitalWrite(pr1mlj,LOW);
digitalWrite(pr2mlj,HIGH);
analogWrite(pamlj,news);
smlj=news;tmlj=1;
//Serial.println(smlj);
  }
    if(newt==2){
digitalWrite(pr2mlj,LOW);
digitalWrite(pr1mlj,HIGH);
analogWrite(pamlj,news);
smlj=news;tmlj=2;
}}
//************************************motor R bala
void mru(byte news,byte newt){
  if(news==0||newt==0){
digitalWrite(pr1mru,LOW);
digitalWrite(pr2mru,LOW);
analogWrite(pamru,0);
tmru=0;smru=0;
  }
  if(newt==1){
digitalWrite(pr1mru,LOW);
digitalWrite(pr2mru,HIGH);
analogWrite(pamru,news);
smru=news;tmru=1;
  }
    if(newt==2){
digitalWrite(pr2mru,LOW);
digitalWrite(pr1mru,HIGH);
analogWrite(pamru,news);
smru=news;tmru=2;
}}
//*****************************************motor L bala
void mlu(byte news,byte newt){
  if(news==0||newt==0){
digitalWrite(pr1mlu,LOW);
digitalWrite(pr2mlu,LOW);
analogWrite(pamlu,0);
tmlu=0;smlu=0;
  }
  if(newt==1){
digitalWrite(pr1mlu,LOW);
digitalWrite(pr2mlu,HIGH);
analogWrite(pamlu,news);
smlu=news;tmlu=1;
  }
    if(newt==2){
digitalWrite(pr2mlu,LOW);
digitalWrite(pr1mlu,HIGH);
analogWrite(pamlu,news);
smlu=news;tmlu=2;
}}
//*******************************************motor aghab
void ma(byte news,byte newt){
  if(news==0||newt==0){
    analogWrite(pr1ma,0);
    analogWrite(pr2ma,0);
  }
  if(newt==1){
analogWrite(pr1ma,0);
analogWrite(pr2ma,news);
sma=news;tma=newt;
  }
}
