#include <Servo.h>
const byte p_right_forward = 2;
const byte p_left_forward = 5;
const byte p_right_up = 3;
const byte p_left_up = 4;
const byte p_back_motor = 6;
const byte p_mosfet=8;
//=============emergency stop================
long last_time=0;
bool first_time = false;
//=============emergency stop================
//=============tune motor balance==============
float tune_volume=0.8;
//=============tune motor balance==============
Servo servor,servol,servour,servoul,servob;
//=====================declear functions================================================================
//-------------------------------------------------
void fb(byte j, float SPEED){
  switch(j){
    case 5 :motor_r(0 ,0);motor_l(0 ,0);break;
    case 2 :motor_r(SPEED,1);motor_l(SPEED ,1);break;
    case 8 :motor_r(SPEED,2);motor_l(SPEED,2);break;
    case 4 :motor_r(SPEED,1);motor_l(SPEED ,2);break;
    case 6 :motor_r(SPEED,2);motor_l(SPEED ,1);break;
    //**********************************************
    case 7 :motor_r(SPEED ,2);motor_l(SPEED/2 ,2);break;
    case 9 :motor_r(SPEED/2 ,2);motor_l(SPEED ,2);break;
    case 1 :motor_r(SPEED ,1);motor_l(SPEED/2 ,1);break;
    case 3 :motor_r(SPEED/2 ,1);motor_l(SPEED ,1);break; 
    default:break;
  }
}
void ud(byte ju , float SPEED){
  switch(ju){
    case 5 :motor_ur(0 ,0);motor_ul(0 ,0);motor_b(0,0);break;
    case 2 :motor_ur(SPEED*tune_volume ,1);motor_ul(SPEED*tune_volume,1);motor_b(SPEED,1);Serial.print(SPEED*tune_volume);
      Serial.println("in pass");
      break;
    case 8 :motor_ur(SPEED*tune_volume ,2);motor_ul(SPEED*tune_volume,2);motor_b(SPEED,2);Serial.print(SPEED*tune_volume);Serial.println("in pass");break;  
    default:break;
}


}
//----------get command----------------
void command_function(byte c,float v){
  switch(c){
    case 1 : mosfet(v);break;
    case 2 :
      change_tune(v);
      delay(10);
      break;
    default :break;
  }
}

//---------motor_r----------------
void motor_r (float SPEED , byte jahat){
if (SPEED==0 || jahat==0){
  servor.writeMicroseconds(1500);
}
if (SPEED!=0 && jahat==1){
  servor.writeMicroseconds(map(SPEED,0,99,1500,1900));
}
if (SPEED!=0 && jahat==2){
  servor.writeMicroseconds(map(SPEED,0,99,1500,1100));
}
}
//------------motor_l----------------------
void motor_l (float SPEED , float  jahat){
if (SPEED==0 || jahat==0){
  servol.writeMicroseconds(1500);
}
if (SPEED!=0 && jahat==1){
  servol.writeMicroseconds(map(SPEED,0,99,1500,1900));
}
if (SPEED!=0 && jahat==2){
  servol.writeMicroseconds(map(SPEED,0,99,1500,1100));
}
}
//------------motor_ur----------------------
void motor_ur (float SPEED , byte jahat){
if (SPEED==0 || jahat==0){
  servour.writeMicroseconds(1500);
}
if (SPEED!=0 && jahat==2){
  servour.writeMicroseconds(map(SPEED,0,99,1500,1900));
  Serial.print(SPEED);
  Serial.println("in motor");
}
if (SPEED!=0 && jahat==1){
  servour.writeMicroseconds(map(SPEED,0,99,1500,1100));
  Serial.print(SPEED);
  Serial.println("in motor");
}
}
//------------motor_ul----------------------
void motor_ul (float SPEED , byte jahat){
if (SPEED==0 || jahat==0){
  servoul.writeMicroseconds(1500);
}
if (SPEED!=0 && jahat==2){
  servoul.writeMicroseconds(map(SPEED,0,99,1500,1900));
}
if (SPEED!=0 && jahat==1){
  servoul.writeMicroseconds(map(SPEED,0,99,1500,1100));
}
}
//------------motor_b----------------------
void motor_b (float SPEED , float  jahat){
if (SPEED==0 || jahat==0){
  servob.writeMicroseconds(1500);
}
if (SPEED!=0 && jahat==1){
  servob.writeMicroseconds(map(SPEED,0,99,1500,1900));
}
if (SPEED!=0 && jahat==2){
  servob.writeMicroseconds(map(SPEED,0,99,1500,1100));
}
}

//-------------------mosfet-------------------
void mosfet(float volume){
  analogWrite(p_mosfet,map(volume,0,99,0,255));
}
//--------------------------------------------
//=============tune motor balance==============
void change_tune(float volume){
  tune_volume = volume / 100.0;
  delay(20);
  Serial.print(tune_volume);
  Serial.println("in function.");
}
//=============tune motor balance==============

//=====================declear functions================================================================

void setup() {
  pinMode(0,INPUT);
  pinMode(1,OUTPUT);
  Serial.begin(9600);
servor.attach(p_right_forward);
servol.attach(p_left_forward);
servour.attach(p_right_up);
servoul.attach(p_left_up);
servob.attach(p_back_motor);
servor.writeMicroseconds(1500);
servol.writeMicroseconds(1500);
servour.writeMicroseconds(1500);
servoul.writeMicroseconds(1500);
servob.writeMicroseconds(1500);
  delay(500);
  Serial.println("hello");
}
void loop(){
    if(Serial.available()){
long input=Serial.parseInt();
Serial.read();
delay(5);
//=============emergency stop================
if (!first_time and input>100000000){
      first_time = true;
    }
//=============emergency stop================
byte wasd=((input-(input%100000000))/100000000)%10;
float speed_forward=((input-(input%1000000))/1000000)%100;
byte up_down=((input-(input%100000))/100000)%10;
float speed_up_down=((input-(input%1000))/1000)%100;
byte command=((input-(input%100))/100)%10;
float vol=((input-(input%1))/1)%100;
fb(wasd,speed_forward);
ud(up_down,speed_up_down);
command_function(command,vol);
//=============emergency stop================
last_time = millis();
//=============emergency stop================
}
//=============emergency stop================
if (millis()-last_time >10000 && first_time){
  fb(5,0);
  ud(2,60);
  command_function(1,99);
}
//=============emergency stop================
}
