#include <Servo.h>  
#include <MPU6050_tockn.h>
#include <Wire.h>
//libs
float tacx=0,tacy=0,tacz=0,ax=0,ay=0,az=0,last_z=0;
byte cmrj=1,ccmrj=2,cmlj=1,ccmlj=2,cmru=1,ccmru=2,cmlu=1,ccmlu=2,cma=1,ccma=2;
byte zarib_yaw=5;
byte zarib_roll=8;
byte zarib_pitch=8;
byte zarib_eslah=5;
//byte zarib_pichesh=2;
byte zarib_gy_yaw=5;
byte zarib_gy_roll=5;
byte zarib_gy_pitch=5;
bool is_stop=true;
bool gy_roll=false;
bool gy_yaw=false;
bool gy_pitch=false;
MPU6050 mpu6050(Wire);
//motor haye jelo R/L
const byte pmrj=8;
const byte pmlj=9;
//motor haye bala R/L
const byte pmru=10;
const byte pmlu=11;
//motor aghab
const byte pma=12;
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
Wire.begin();
mpu6050.begin();
mpu6050.calcGyroOffsets(false);
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
stop_signal();
 }
void loop(){

      
  char jht;int srt;char blp;int sblp;String tempin;       //blp is bala & paieen and sblp is sorat and srt is sorat & jht is jahat
  //Serial.println(smrj+tmrj+smlj+tmlj+smru+tmru+smlu+tmlu+sma+tma);
  if(Serial.available()>0){
  String input = Serial.readStringUntil('>');
  if(input[0]!='g'){
jht = input[0];
blp=input[3];
tempin=((String)input[1]+input[2]);
 srt=(tempin.toInt());
  tempin=((String)input[4]+input[5]);
 sblp=(tempin.toInt());
 gy_on_off(input[6]);
   delay(5);
    Serial.read();
    jrl(jht,srt,true);ud(blp,sblp,true);change_ang();
//    Serial.println((String)"motor state");

    }
    if(input[0]=='g'){
      gcommand(input);
    }
    }
    if(change_ang()){
    z_static(gy_yaw);
    xy_static();
    }

}
void jrl(char j,byte ri,bool at){
switch(j){
  case 'w':mrj(ri,cmrj);mlj(ri,cmlj);if(at)move_signal();break;
  case 's':mrj(ri,ccmrj);mlj(ri,ccmlj);if(at)move_signal();break;
  case 'd':mrj(ri,ccmrj);mlj(ri,cmlj);if(at)move_signal();break;
  case 'a':mrj(ri,cmrj);mlj(ri,ccmlj);if(at)move_signal();break;
//  *******************************
  case 'q':mlj(ri/2,cmlj);mrj(ri,cmrj);if(at)move_signal();break;
  case 'e':mlj(ri,cmlj);mrj(ri/2,cmrj);if(at)move_signal();break;
  case 'z':mrj(ri,ccmrj);mlj(ri/2,ccmlj);if(at)move_signal();break;
  case 'c':mlj(ri,ccmlj);mrj(ri/2,ccmrj);if(at)move_signal();break;
//  **************************************
  case 'x':mrj(0,0);mlj(0,0);if(at)stop_signal();break;
  default:break;
}
  }
//  ***********************************
void ud(char ju,byte ru,bool at){
  switch(ju){
    case 'u':mru(ru*(zarib_eslah/10),cmru);mlu(ru*0.6,cmlu);ma(ru,cma);break;
    case 'j':mru(ru*(zarib_eslah/10),ccmru);mlu(ru*0.6,ccmlu);ma(ru,ccma);break;
    case 'i':mru(0,0);mlu(0,0);ma(0,0);break;
    default:break;
  }
  
}


//*********************************************************************
void gy_on_off(char check){
  switch(check){
    case 'r':if(gy_roll==true){gy_roll=false;Serial.println("gyro roll is off");}else {gy_roll=true;Serial.println("gyro roll is on");};break;
    case 'y':if(gy_yaw==true){gy_yaw=false;Serial.println("gyro yaw is off");}else {gy_yaw=true;Serial.println("gyro yaw is on");};break;
    case 'p':if(gy_pitch==true){gy_pitch=false;Serial.println("gyro pitch is off");}else {gy_pitch=true;Serial.println("gyro pitch is on");};break;
    default:break;
  }
}
//*********************************************************************
void gcommand(String in){
  if(in[1]=='r'){
    change_motor_wire(in[2]);
  }
  if(in[1]=='a'){
    eslah_zarib(in[2],in[3]);
  }
  if(in[1]=='m'&&in[2]=='a'){
    eslah_zarib_motor_up(in[3]);
  }
  if(in[1]=='y'){
    zarib_gy(in[2],in[3]);
  }
}
//**********************************************************************
void zarib_gy(char ang,char zar){
  String za=((String)zar);
  byte zarbb=za.toInt();
  switch(ang){
    case 'y':zarib_gy_yaw=zarbb;break;
    case 'r':zarib_gy_roll=zarbb;break;
    case 'p':zarib_gy_pitch=zarbb;break;
  }
}
//**********************************************************************
void eslah_zarib(char mo,char ang){
  String an=((String)ang);
  byte angl=an.toInt();
  switch(mo){
    case 'y':zarib_yaw=angl;break;
    case 'r':zarib_roll=angl;break;
    case 'p':zarib_pitch=angl;break;
  }
}
//**********************************************************************
void eslah_zarib_motor_up(char k){
 String z=((String)k);
 zarib_eslah=z.toInt(); 
}
//**********************************************************************

void change_motor_wire(char motor){
  switch(motor){
    case '1':if(cmlu==1){cmlu=2;ccmlu=1;break;}if(cmlu==2){cmlu=1;ccmlu=2;break;}break;
    case '2':if(cmru==1){cmru=2;ccmru=1;break;}if(cmru==2){cmru=1;ccmru=2;break;}break;
    case '3':if(cmlj==1){cmlj=2;ccmlj=1;break;}if(cmlj==2){cmlj=1;ccmlj=2;break;}
    case '4':if(cmrj==1){cmrj=2;ccmrj=1;break;}if(cmrj==2){cmrj=1;ccmrj=2;break;}break;
    case '5':if(cma==1){cma=2;ccma=1;break;}if(cma==2){cma=1;ccma=2;break;}break;
    default:break;
  }
}
//********************************************************************
void lock_z(){
  last_z=az;
}
void stop_signal(){
is_stop=true;
  change_ang();
last_z=az;
Serial.println((String)"stop signall lock at : "+last_z);
}
void move_signal(){
  is_stop=false;
  change_ang();
  Serial.println((String)"movesignall lock at : "+last_z);
}
bool change_ang(){
    mpu6050.update();
    float acx=(mpu6050.getAccX());
    float acy=(mpu6050.getAccY());
    float acz=(mpu6050.getAccZ());
    if(abs(acx-tacx)>0.05 || abs(acy-tacy)>0.05 || abs(acz-tacz)>0.05){
      ax=(mpu6050.getAngleX());
      ay=(mpu6050.getAngleY());
      az=(mpu6050.getAngleZ());
      tacx=acx;tacy=acy;tacz=acz;  
      return true;
    }
      else{
      return false;
      }
}
char xy_state(){
  if(abs(ax)<7 && abs(ay)<7){
  return 'v';
  }
  if(ax>7 && abs(ay)<7){
  return 's';
  }
  if(ax<-7 && abs(ay)<7){
  return 'w';
  }
  if(ay>7 && abs(ax)<7){
  return 'd';
  }
  if(ay<-7 && abs(ax)<7){
  return 'a';
  }
  if(ax>7 && ay>7){
  return 'c';
  }
  if(ax>7 && ay<-7){
  return 'z';
  }
  if(ay<-7 && ax<-7){
  return 'q';
  }
  if(ay>7 && ax<-7){
  return 'e';
  }
}
void xy_static(){
    if(is_stop){
  switch(xy_state()){
  case 'v':if(gy_roll || gy_pitch){mru(0,0);mlu(0,0);Serial.println("vaz taadol");}break;//khamooshi motor ha
  case 'w':if(gy_pitch){mru(xy_static_speed(abs(ax))*(zarib_gy_pitch/5),cmru);mlu(xy_static_speed(abs(ax))*(zarib_gy_pitch/5),cmlu);Serial.println((String)"vaz w motor left up speed= "+xy_static_speed(abs(ax)));};break;
  case 'a':if(gy_roll){mru(xy_static_speed(abs(ay))*(zarib_gy_roll/5),ccmru);mlu(xy_static_speed(abs(ay))*(zarib_gy_roll/5),cmlu);Serial.println("vaz a");}break;
  case 's':if(gy_pitch){mru(xy_static_speed(abs(ax))*(zarib_gy_pitch/5),ccmru);mlu(xy_static_speed(abs(ax))*(zarib_gy_pitch/5),ccmlu);Serial.println("vaz s");}break;
  case 'd':if(gy_roll){mru(xy_static_speed(abs(ay))*(zarib_gy_roll/5),cmru);mlu(xy_static_speed(abs(ay))*(zarib_gy_roll/5),ccmlu);Serial.println("vaz d");}break;
  case 'q':if(gy_pitch&&gy_roll){}break;
  case 'e':if(gy_pitch&&gy_roll){}break;
  case 'z':if(gy_pitch&&gy_roll){}break;
  case 'c':if(gy_pitch&&gy_roll){}break;
  default:break;
  }
  }
}
//***************************************************************
float narm(float s){
  delay(5);
  return s-1;
}
float z_static_speed(float angle){
  angle=abs(angle);
  return map(angle,1,180,9,70);
  
}
float xy_static_speed(float angle){
  angle=abs(angle);
  return map(angle,8,45,15,80);
  
}
//***********************************************************
void z_static(bool check){
if(check && is_stop && abs(az-last_z)>zarib_yaw){
    if(az-last_z > 0){
      //harekat 
      Serial.println((String)"d "+z_static_speed(az-last_z));
      jrl('d',z_static_speed(az-last_z)*(zarib_gy_yaw/5),false);
  //Serial.println((String)"charkhesh rast") ;   
    }
    if(az-last_z<0){
//harekat
Serial.println((String)"a "+z_static_speed(az-last_z));     
jrl('a',z_static_speed(abs(az-last_z))*(zarib_gy_yaw/5),false);
//Serial.println((String)"charkhesh chap") ;
    }
  }
  if(check && is_stop && abs(az-last_z)<=zarib_yaw){
//stop
Serial.println((String)"x "+z_static_speed(az-last_z));
jrl('x',0,false);
//Serial.println((String)"stop charkhesh");
  }
}
//***********************************************************************
//*************************************motor R jolo
void mrj(byte news,byte newt){
  if(news==0||newt==0){
    mtrj.writeMicroseconds(1500);
tmrj=0;smrj=0;

  }
  if(newt==1&&news!=0){
    mtrj.writeMicroseconds(map(news,1,99,1540,2000));
    Serial.println((String)"right motor speed= "+news+" state "+newt);
smrj=news;tmrj=1;
  }
    if(newt==2&&news!=0){
    mtrj.writeMicroseconds(map(news,1,99,1460,1000));
    Serial.println((String)"right motor speed= "+news+" state "+newt);
smrj=news;tmrj=2;
}

}
//*************************************motor L jolo
void mlj(byte news,byte newt){
 if(news==0||newt==0){
    mtlj.writeMicroseconds(1500);
tmlj=0;smlj=0;

  }
  if(newt==1&&news!=0&&news!=0){
    mtlj.writeMicroseconds(map(news,1,99,1540,1900));
    Serial.println((String)"left motor speed= "+news+" state "+newt);
smlj=news;tmlj=1;
  }
    if(newt==2&&news!=0&&news!=0){
    mtlj.writeMicroseconds(map(news,1,99,1460,1100));
    Serial.println((String)"left motor speed= "+news+" state "+newt);
smlj=news;tmlj=2;
}
}
//************************************motor R bala
void mru(byte news,byte newt){
 if(news==0||newt==0){
    mtru.writeMicroseconds(1500);
tmru=0;smru=0;

  }
  if(newt==1&&news!=0){
    mtru.writeMicroseconds(map(news,1,99,1540,2000));
smru=news;tmru=1;
  }
    if(newt==2&&news!=0){
    mtru.writeMicroseconds(map(news,1,99,1460,1000));
smru=news;tmru=2;
}
}
//*****************************************motor L bala
void mlu(byte news,byte newt){
if(news==0||newt==0){
    mtlu.writeMicroseconds(1500);
tmlu=0;smlu=0;

  }
  if(newt==1&&news!=0){
    mtlu.writeMicroseconds(map(news,1,99,1540,2000));
smlu=news;tmlu=1;
  }
    if(newt==2&&news!=0){
    mtlu.writeMicroseconds(map(news,1,99,1460,1000));
smlu=news;tmlu=2;
}
}
//*******************************************motor aghab
void ma(byte news,byte newt){
if(news==0||newt==0){
    mta.writeMicroseconds(1500);
tma=0;sma=0;

  }
  if(newt==1&&news!=0){
    mta.writeMicroseconds(map(news,1,99,1540,2000));
sma=news;tma=1;
  }
    if(newt==2&&news!=0){
    mta.writeMicroseconds(map(news,1,99,1460,1000));
sma=news;tma=2;
}
}
