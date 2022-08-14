#include <Servo.h>

byte servoPin = 9; // signal pin for the ESC.
byte potentiometerPin = A1; // analog input pin for the potentiometer.
Servo servo;

void setup() {
  Serial.begin(9600);
servo.attach(servoPin);
servo.writeMicroseconds(1500); // send "stop" signal to ESC. Also necessary to arm the ESC.

delay(7000); // delay to allow the ESC to recognize the stopped signal.
}

void loop() {
if(Serial.available()>0){
int pwmVal =Serial.parseInt();
Serial.read();
//int pwmVal = map(potVal,0, 1023, 1100, 1900); // maps potentiometer values to PWM value.

servo.writeMicroseconds(pwmVal); // Send signal to ESC.
}}
