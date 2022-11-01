#include <MPU6050_tockn.h>
#include <Servo.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 28
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// libs
float tacx = 0, tacy = 0, tacz = 0, ax = 0, ay = 0, az = 0, last_z = 0;
byte cmrj = 1, ccmrj = 2;
byte cmlj = 1, ccmlj = 2;
byte cmru = 2, ccmru = 1;
byte cmlu = 1, ccmlu = 2;
byte cma = 2, ccma = 1;
byte zarib_yaw = 5;
byte zarib_roll = 8;
byte zarib_pitch = 8;
byte zarib_eslah = 5;
// byte zarib_pichesh=2;
char pitch_state = 'v';
//===============================
byte zarib_gy_yaw = 5;
byte zarib_gy_roll = 5;
byte zarib_gy_pitch = 5;
bool is_stop = true;
bool gy_roll = false;
bool gy_yaw = false;
bool gy_pitch = false;
bool gy_vazne = false;
//byte p_ms = 13;
byte p_r1ms = 9;
byte p_r2ms = 10;
MPU6050 mpu6050(Wire);
byte led_pin = 7;
// motor haye jelo R/L
const byte pmrj = 2;
const byte pmlj = 5;
// motor haye bala R/L
const byte pmru = 3;
const byte pmlu = 4;
//.......................................................
const byte led_orange = 12;
const byte led_white = 13;

// motor aghab
const byte pma = 6;
// motor state and speed
float smrj = 0;
byte tmrj = 0;
float smlj = 0;
byte tmlj = 0;
float smru = 0;
byte tmru = 0;
float smlu = 0;
byte tmlu = 0;
float sma = 0;
byte tma = 0;
//last commands
char last_wasd = 'x';
float last_wasd_speed = 0;
float taghe=0.5;
float zarib_p_sport=0.7;
float zarib_jelobandi=0.25;
char last_ud = 'i';
float last_ud_speed = 0;
//****************************************
Servo mtrj, mtlj, mtlu, mtru, mta;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  sensors.begin();
  mpu6050.begin();
  pinMode(led_white, OUTPUT);
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
void loop() {

  char jht;
  int srt;
  char blp;
  int sblp;
  String tempin;  // blp is bala & paieen and sblp is sorat and srt is sorat &
  // jht is jahat
  // Serial.println(smrj+tmrj+smlj+tmlj+smru+tmru+smlu+tmlu+sma+tma);
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('>');
    if (input[0] != 'g') {
      jht = input[0];
      blp = input[3];
      tempin = ((String)input[1] + input[2]);
      srt = (tempin.toInt());
      tempin = ((String)input[4] + input[5]);
      sblp = (tempin.toInt());
      last_wasd = jht;
      last_wasd_speed = srt;
      if (blp == 'u' || blp == 'j' || blp == 'i' || blp == 'w' || blp == 's' || blp == 'x' || blp == 'v' || blp == 'b') {
        last_ud = blp;
        last_ud_speed = sblp;
      }
      one_char(input[6]);
      //      gy_on_off(input[6]);
      delay(5);
      Serial.read();
      jrl(jht, srt, true);
      ud(blp, sblp, true);
      change_ang();
      //    Serial.println((String)"motor state");
    }
    if (input[0] == 'g') {
      gcommand(input);
    }
  }
  if (change_ang()) {
    z_static(gy_yaw);
    xy_static();
  }
  if (gy_vazne) {
    vazne();
  }
}
void jrl(char j, byte ri, bool at) {
  switch (j) {
    case 'w':
      mrj(ri, cmrj);
      mlj(ri, cmlj);
      if (at)
        move_signal();
      break;
    case 's':
      mrj(ri, ccmrj);
      mlj(ri, ccmlj);
      if (at)
        move_signal();
      break;
    case 'd':
      mrj(ri, ccmrj);
      mlj(ri, cmlj);
      if (at)
        move_signal();
      break;
    case 'a':
      mrj(ri, cmrj);
      mlj(ri, ccmlj);
      if (at)
        move_signal();
      break;
    //  *******************************
    case 'q':
      mlj(ri / 3, cmlj);
      mrj(ri, cmrj);
      if (at)
        move_signal();
      break;
    case 'e':
      mlj(ri, cmlj);
      mrj(ri / 3, cmrj);
      if (at)
        move_signal();
      break;
    case 'z':
      mrj(ri, ccmrj);
      mlj(ri / 3, ccmlj);
      if (at)
        move_signal();
      break;
    case 'c':
      mlj(ri, ccmlj);
      mrj(ri / 3, ccmrj);
      if (at)
        move_signal();
      break;
    //  **************************************
    case 'x':
      mrj(0, 0);
      mlj(0, 0);
      if (at)
        stop_signal();
      break;
    //+++++++++++++++++===============++++++++++++++++++++++++++++======+++++++++++====================
    case 'i':
      mrj(ri-zarib_jelobandi, cmrj);
      mlj(ri, cmlj);
      mru(ri, cmru);
      mlu(ri, cmlu);
      if (at)
        move_signal();
      break;
    case 'k':
      mrj(ri, ccmrj);
      mlj(ri, ccmlj);
      mru(ri, ccmru);
      mlu(ri, ccmlu);
      if (at)
        move_signal();
      break;
    case 'l':
      mrj(ri, ccmrj);
      mlj(ri, cmlj);
      mru(ri, ccmru);
      mlu(ri, cmlu);
      if (at)
        move_signal();
      break;
    case 'j':
      mrj(ri, cmrj);
      mlj(ri, ccmlj);
      mru(ri, cmru);
      mlu(ri, ccmlu);
      if (at)
        move_signal();
      break;
    //  *******************************
    case 'u':
      mlj(ri / 3, cmlj);
      mrj(ri, cmrj);
      mlu(ri / 3, cmlu);
      mru(ri, cmru);
      if (at)
        move_signal();
      break;
    case 'o':
      mlj(ri, cmlj);
      mrj(ri / 3, cmrj);
      mlu(ri, cmlu);
      mru(ri / 3, cmru);
      if (at)
        move_signal();
      break;
    case 'n':
      mrj(ri, ccmrj);
      mlj(ri / 3, ccmlj);
      mru(ri, ccmru);
      mlu(ri / 3, ccmlu);
      if (at)
        move_signal();
      break;
    case 'm':
      mlj(ri, ccmlj);
      mrj(ri / 3, ccmrj);
      mlu(ri, ccmlu);
      mru(ri / 3, ccmru);
      if (at)
        move_signal();
      break;
    //  **************************************
    case 'r':
      mlj(ri *zarib_p_sport, cmlj);
      mrj(ri, cmrj);
      mlu(ri *zarib_p_sport, cmlu);
      mru(ri, cmru);
      if (at)
        move_signal();
      break;
    case 't':
      mlj(ri, cmlj);
      mrj(ri *zarib_p_sport, cmrj);
      mlu(ri, cmlu);
      mru(ri *zarib_p_sport, cmru);
      if (at)
        move_signal();
      break;
    case 'f':
      mrj(ri, ccmrj);
      mlj(ri *zarib_p_sport, ccmlj);
      mru(ri, ccmru);
      mlu(ri *zarib_p_sport, ccmlu);
      if (at)
        move_signal();
      break;
    case 'g':
      mlj(ri, ccmlj);
      mrj(ri *zarib_p_sport, ccmrj);
      mlu(ri, ccmlu);
      mru(ri *zarib_p_sport , ccmru);
      if (at)
        move_signal();
      break;
    //==========================================
    case 'p':
      mrj(0, 0);
      mlj(0, 0);
      mru(0, 0);
      mlu(0, 0);
      if (at)
        stop_signal();
      break;
    //++++++++++++++++=============++++++++++++++===========++++++++++++++++++
    default:
      break;
  }
}
//  ***********************************
void ud(char ju, byte ru, bool at) {
  char sta;
  switch (ju) {
    case 'u':
      mru(ru * 0.6, cmru);
      mlu(ru * 0.6, cmlu);
      ma(ru, cma);
      break;
    case 'j':
      mru(ru * 0.6, ccmru);
      mlu(ru * 0.6, ccmlu);
      ma(ru, ccma);
      break;
    case 'i':
      mru(0, 0);
      mlu(0, 0);
      ma(0, 0);
      break;
    case 'w':
      ma(ru, cma);
      break;
    case 's':
      ma(ru, ccma);
      break;
    case 'x':
      ma(0, 0);
      break;
    case 'v':
      ma(sma + ru, cma);
      mru(smru + ru, cmru);
      mlu(smlu + ru, cmlu);
      Serial.println((String) "dor motor afzayesh yaft be :" + smru + " ba taghieer +" + ru);
      break;
    case 'b':
      ma(sma - ru, cma);
      mru(smru - ru, cmru);
      mlu(smlu - ru, cmlu);
      Serial.println((String) "dor motor kahesh yaft be :" + smru + " ba taghieer -" + ru);
      break;
    case 'p':
      {
        ma(sma, tma);
        pitch_state = xy_state();
        if (pitch_state == 'v' || pitch_state == 's' || pitch_state == 'z' || pitch_state == 'c') {
          if (tmlu == 0) {
            mlu(smlu + 0.5, ccmlu);
            mru(smru + 0.5, ccmru);
          }
          if (tmlu == ccmlu) {
            mlu(smlu + 0.5, ccmlu);
            mru(smru + 0.5, ccmru);
          }
          if (tmlu == cmlu) {
            mlu(smlu - 0.5, cmlu);
            mru(smru - 0.5, cmru);
          }
        }
        if (pitch_state == 'w' || pitch_state == 'q' || pitch_state == 'e') {
          if ((ru < abs(ax)) && ((abs(ax) - ru) > 5)) {
            if (tmlu == 0) {
              mlu(smlu + 0.5, ccmlu);
              mru(smru + 0.5, ccmru);
            }
            if (tmlu == ccmlu) {
              mlu(smlu + 0.5, ccmlu);
              mru(smru + 0.5, ccmru);
            }
            if (tmlu == cmlu) {
              mlu(smlu - 0.5, cmlu);
              mru(smru - 0.5, cmru);
            }
          }
          if ((ru > abs(ax)) && ((abs(ax) - ru) < 5)) {
            if (tmlu == 0) {
              mlu(smlu + 0.5, cmlu);
              mru(smru + 0.5, cmru);
            }
            if (tmlu == ccmlu) {
              mlu(smlu - 0.5, ccmlu);
              mru(smru - 0.5, ccmru);
            }
            if (tmlu == cmlu) {
              mlu(smlu + 0.5, cmlu);
              mru(smru + 0.5, cmru);
            }
          }
        }
      }
      break;
    case 'n':
      {
        ma(sma, tma);
        pitch_state = xy_state();
        if (pitch_state == 'v' || pitch_state == 'w' || pitch_state == 'q' || pitch_state == 'e') {
          if (tmlu == 0) {
            mlu(smlu + 0.5, cmlu);
            mru(smru + 0.5, cmru);
          }
          if (tmlu == cmlu) {
            mlu(smlu + 0.5, cmlu);
            mru(smru + 0.5, cmru);
          }
          if (tmlu == ccmlu) {
            mlu(smlu - 0.5, ccmlu);
            mru(smru - 0.5, ccmru);
          }
        }
        if (pitch_state == 's' || pitch_state == 'z' || pitch_state == 'c') {
          if ((ru < abs(ax)) && ((abs(ax) - ru) > 5)) {
            if (tmlu == 0) {
              mlu(smlu + 0.5, cmlu);
              mru(smru + 0.5, cmru);
            }
            if (tmlu == cmlu) {
              mlu(smlu + 0.5, cmlu);
              mru(smru + 0.5, cmru);
            }
            if (tmlu == ccmlu) {
              mlu(smlu - 0.5, ccmlu);
              mru(smru - 0.5, ccmru);
            }
          }
          if ((ru > abs(ax)) && ((abs(ax) - ru) < 5)) {
            if (tmlu == 0) {
              mlu(smlu + 0.5, ccmlu);
              mru(smru + 0.5, ccmru);
            }
            if (tmlu == cmlu) {
              mlu(smlu - 0.5, cmlu);
              mru(smru - 0.5, cmru);
            }
            if (tmlu == ccmlu) {
              mlu(smlu + 0.5, ccmlu);
              mru(smru + 0.5, ccmru);
            }
          }
        }
      }
      break;
    case 't':
      pitch_state = xy_state();
      if (pitch_state == 'v' || pitch_state == 's' || pitch_state == 'z' || pitch_state == 'c') {
        if (tma == 0) {
          ma(sma + 0.5, cma);
        }
        if (tma == cma) {
          ma(sma + 0.5, cma);
        }
        if (tma == ccma) {
          ma(sma - 0.5, ccma);
        }
      }
      if (pitch_state == 'w' || pitch_state == 'q' || pitch_state == 'e') {
        if ((ru < abs(ax)) && ((abs(ax) - ru) > 5)) {
          if (tma == 0) {
            ma(sma + 0.5, cma);
          }
          if (tma == cma) {
            ma(sma + 0.5, cma);
          }
          if (tma == ccma) {
            ma(sma - 0.5, ccma);
          }
        }
        if ((ru > abs(ax)) && ((abs(ax) - ru) < 5)) {
          if (tmlu == 0) {
            ma(sma + 0.5, ccma);
          }
          if (tma == cma) {
            ma(sma - 0.5, cma);
          }
          if (tma == ccma) {
            ma(sma + 0.5, ccma);
          }
        }
      }
      break;
    case 'g':
      pitch_state = xy_state();
      if (pitch_state == 'v' || pitch_state == 'w' || pitch_state == 'q' || pitch_state == 'e') {
        if (tma == 0) {
          ma(sma + 0.5, ccma);
        }
        if (tma == ccma) {
          ma(sma + 0.5, ccma);
        }
        if (tma == cma) {
          ma(sma - 0.5, cma);
        }
      }
      if (pitch_state == 's' || pitch_state == 'z' || pitch_state == 'c') {
        if ((ru < abs(ax)) && ((abs(ax) - ru) > 5)) {
          if (tma == 0) {
            ma(sma + 0.5, ccma);
          }
          if (tma == ccma) {
            ma(sma + 0.5, ccma);
          }
          if (tma == cma) {
            ma(sma - 0.5, cma);
          }
        }
        if ((ru > abs(ax)) && ((abs(ax) - ru) < 5)) {
          if (tmlu == 0) {
            ma(sma + 0.5, cma);
          }
          if (tma == ccma) {
            ma(sma - 0.5, ccma);
          }
          if (tma == cma) {
            ma(sma + 0.5, cma);
          }
        }
      }
      break;
    default:
      break;
  }
}
//=====================================================================
void vazne() {
  char stat;
  //  Serial.println(stat);
  if (gy_vazne) {
    //    Serial.println("in vazne loop");
    stat = xy_state();
    if (stat == 'w' || stat == 'q' || stat == 'e') {
      mru(smru + abs(ax) / 7, cmru);
      mlu(smlu + abs(ax) / 7, cmlu);
      Serial.println((String) "dor motor afzayesh yaft be : " + smru);
    }
    if (stat == 's' || stat == 'z' || stat == 'c') {
      mru(smru - abs(ax) / 7, cmru);
      mlu(smlu - abs(ax) / 7, cmlu);
      Serial.println((String) "dor motor kahesh yaft be : " + smru);
    }
  }
}
//*********************************************************************
void one_char(char in) {
  String tempo;
  byte vol = 0;
  if (!(in == '0' || in == '1' || in == '2' || in == '3' || in == '4' || in == '5' || in == '6' || in == '7' || in == '8' || in == '9')) {
    gy_on_off(in);
    motor_sport(in);
    Serial.println("gy in");
  }

  if (in == '0' || in == '1' || in == '2' || in == '3' || in == '4' || in == '5' || in == '6' || in == '7' || in == '8' || in == '9') {
    tempo = ((String)in);
    Serial.println("its in");
    vol = (tempo.toInt());
    led_volume(vol);
  }
}
//*********************************************************************
void gy_on_off(char check) {
  switch (check) {
    case 'r':
      if (gy_roll == true) {
        gy_roll = false;
        Serial.println("gyro roll is off");
      } else {
        gy_roll = true;
        Serial.println("gyro roll is on");
      };
      break;
    case 'y':
      if (gy_yaw == true) {
        gy_yaw = false;
        Serial.println("gyro yaw is off");
      } else {
        gy_yaw = true;
        Serial.println("gyro yaw is on");
      };
      break;
    case 'p':
      if (gy_pitch == true) {
        gy_pitch = false;
        Serial.println("gyro pitch is off");
      } else {
        gy_pitch = true;
        Serial.println("gyro pitch is on");
      };
      break;
    case 'v':
      if (gy_vazne == true) {
        gy_vazne = false;
        Serial.println("gyro vazne is off");
      } else {
        gy_vazne = true;
        Serial.println("gyro vazne is on");
      };
      break;
    default:
      break;
  }
}
//*********************************************************************
void gcommand(String in) {
  if (in[1] == 'r') {
    change_motor_wire(in[2]);
  }
  if (in[1] == 'j') {
    zarib_taadol(in[2], in[3]);
  }
  if (in[1] == 'm' && in[2] == 'a') {
    eslah_zarib_motor_up(in[3]);
  }
  if (in[1] == 'z') {
    zarib_sport(in[2], in[3]);
  }
  if (in[1] == 'l') {
    led_control(in[2], in[3]);
  }
  if (in[1] == 't') {
    Serial.println((String) "temp" + get_temp() + "temp");
  }
}
//**********************************************************************
void zarib_gy(char ang, char zar) {
  String za = ((String)zar);
  byte zarbb = za.toInt();
  switch (ang) {
    case 'y':
      zarib_gy_yaw = zarbb;
      break;
    case 'r':
      zarib_gy_roll = zarbb;
      break;
    case 'p':
      zarib_gy_pitch = zarbb;
      break;
  }
}
//======================================================================
void led_control(char led_type, byte volume) {
  switch (led_type) {
    case 'o':
      orange_led(volume);
      break;
    case 'w':
      white_led(volume);
      break;
    case 'b':
      orange_led(volume);
      white_led(volume);
      break;
    default:
      break;
  }
}
//**********************************************************************
void eslah_zarib(char mo, char ang) {
  String an = ((String)ang);
  byte angl = an.toInt();
  switch (mo) {
    case 'y':
      zarib_yaw = angl;
      break;
    case 'r':
      zarib_roll = angl;
      break;
    case 'p':
      zarib_pitch = angl;
      break;
  }
}
//**********************************************************************
void eslah_zarib_motor_up(char k) {
  String z = ((String)k);
  zarib_eslah = z.toInt();
}
//**********************************************************************

void change_motor_wire(char motor) {
  switch (motor) {
    case '1':
      if (cmlu == 1) {
        cmlu = 2;
        ccmlu = 1;
        break;
      }
      if (cmlu == 2) {
        cmlu = 1;
        ccmlu = 2;
        break;
      }
      break;
    case '2':
      if (cmru == 1) {
        cmru = 2;
        ccmru = 1;
        break;
      }
      if (cmru == 2) {
        cmru = 1;
        ccmru = 2;
        break;
      }
      break;
    case '3':
      if (cmlj == 1) {
        cmlj = 2;
        ccmlj = 1;
        break;
      }
      if (cmlj == 2) {
        cmlj = 1;
        ccmlj = 2;
        break;
      }
    case '4':
      if (cmrj == 1) {
        cmrj = 2;
        ccmrj = 1;
        break;
      }
      if (cmrj == 2) {
        cmrj = 1;
        ccmrj = 2;
        break;
      }
      break;
    case '5':
      if (cma == 1) {
        cma = 2;
        ccma = 1;
        break;
      }
      if (cma == 2) {
        cma = 1;
        ccma = 2;
        break;
      }
      break;
    default:
      break;
  }
}
//********************************************************************
void lock_z() {
  last_z = az;
}
void stop_signal() {
  is_stop = true;
  change_ang();
  last_z = az;
  Serial.println((String) "stop signall lock at : " + last_z);
}
void move_signal() {
  is_stop = false;
  change_ang();
  Serial.println((String) "movesignall start at : " + last_z);
}
bool change_ang() {
  mpu6050.update();
  float acx = (mpu6050.getAccX());
  float acy = (mpu6050.getAccY());
  float acz = (mpu6050.getAccZ());
  if (abs(acx - tacx) > 0.05 || abs(acy - tacy) > 0.05 || abs(acz - tacz) > 0.05) {
    ax = (mpu6050.getAngleX());
    ay = (mpu6050.getAngleY());
    az = (mpu6050.getAngleZ());
    tacx = acx;
    tacy = acy;
    tacz = acz;
    return true;
  } else {
    return false;
  }
}
char xy_state() {
  if (abs(ax) < 7 && abs(ay) < 7) {
    return 'v';
  }
  if (ax > 7 && abs(ay) < 7) {
    return 's';
  }
  if (ax < -7 && abs(ay) < 7) {
    return 'w';
  }
  if (ay > 7 && abs(ax) < 7) {
    return 'd';
  }
  if (ay < -7 && abs(ax) < 7) {
    return 'a';
  }
  if (ax > 7 && ay > 7) {
    return 'c';
  }
  if (ax > 7 && ay < -7) {
    return 'z';
  }
  if (ay < -7 && ax < -7) {
    return 'q';
  }
  if (ay > 7 && ax < -7) {
    return 'e';
  }
}
void xy_static() {
  if (is_stop) {
    switch (xy_state()) {
      case 'v':
        if (gy_roll || gy_pitch) {
          mru(0, 0);
          mlu(0, 0);
          Serial.println("vaz taadol");
        }
        break;  // khamooshi motor ha
      case 'w':
        if (gy_pitch) {
          mru(xy_static_speed(abs(ax)) * (zarib_gy_pitch / 5), cmru);
          mlu(xy_static_speed(abs(ax)) * (zarib_gy_pitch / 5), cmlu);
          Serial.println((String) "vaz w motor left up speed= " + xy_static_speed(abs(ax)));
        };
        break;
      case 'a':
        if (gy_roll) {
          mru(xy_static_speed(abs(ay)) * (zarib_gy_roll / 5), ccmru);
          mlu(xy_static_speed(abs(ay)) * (zarib_gy_roll / 5), cmlu);
          Serial.println("vaz a");
        }
        break;
      case 's':
        if (gy_pitch) {
          mru(xy_static_speed(abs(ax)) * (zarib_gy_pitch / 5), ccmru);
          mlu(xy_static_speed(abs(ax)) * (zarib_gy_pitch / 5), ccmlu);
          Serial.println("vaz s");
        }
        break;
      case 'd':
        if (gy_roll) {
          mru(xy_static_speed(abs(ay)) * (zarib_gy_roll / 5), cmru);
          mlu(xy_static_speed(abs(ay)) * (zarib_gy_roll / 5), ccmlu);
          Serial.println("vaz d");
        }
        break;
      case 'q':
        if (gy_pitch && gy_roll) {
        }
        break;
      case 'e':
        if (gy_pitch && gy_roll) {
        }
        break;
      case 'z':
        if (gy_pitch && gy_roll) {
        }
        break;
      case 'c':
        if (gy_pitch && gy_roll) {
        }
        break;
      default:
        break;
    }
  }
}
//***************************************************************
float narm(float s) {
  delay(5);
  return s - 1;
}
float z_static_speed(float angle) {
  angle = abs(angle);
  return map(angle, 1, 180, 9, 70);
}
float xy_static_speed(float angle) {
  angle = abs(angle);
  return map(angle, 8, 45, 15, 80);
}
//***********************************************************
void z_static(bool check) {
  if (check && is_stop && abs(az - last_z) > zarib_yaw) {
    if (az - last_z > 0) {
      // harekat
      Serial.println((String) "d " + z_static_speed(az - last_z));
      jrl('d', z_static_speed(az - last_z) * (10 / 5), false);
      // Serial.println((String)"charkhesh rast") ;
    }
    if (az - last_z < 0) {
      // harekat
      Serial.println((String) "a " + z_static_speed(az - last_z));
      jrl('a', z_static_speed(abs(az - last_z)) * (10 / 5), false);
      // Serial.println((String)"charkhesh chap") ;
    }
  }
  if (check && is_stop && abs(az - last_z) <= zarib_yaw) {
    // stop
    Serial.println((String) "x " + z_static_speed(az - last_z));
    jrl('x', 0, false);
    // Serial.println((String)"stop charkhesh");
  }
}
//***********************************************************************
//*************************************motor R jolo
void mrj(float news, byte newt) {
  if (news < 0) {
    news = 0;
  }
  if (news > 99) {
    news = 99;
  }

  if (news == 0 || newt == 0) {
    mtrj.writeMicroseconds(1500);
    tmrj = 0;
    smrj = 0;
  }
  if (newt == 1 && news != 0) {
    mtrj.writeMicroseconds(map(news, 1, 99, 1540, 1900));
    Serial.println((String) "right motor speed= " + news + " state " + newt);
    smrj = news;
    tmrj = 1;
  }
  if (newt == 2 && news != 0) {
    mtrj.writeMicroseconds(map(news, 1, 99, 1460, 1100));
    Serial.println((String) "right motor speed= " + news + " state " + newt);
    smrj = news;
    tmrj = 2;
  }
}
//*************************************motor L jolo
void mlj(float news, byte newt) {
  if (news < 0) {
    news = 0;
  }
  if (news > 99) {
    news = 99;
  }

  if (news == 0 || newt == 0) {
    mtlj.writeMicroseconds(1500);
    tmlj = 0;
    smlj = 0;
  }
  if (newt == 1 && news != 0 && news != 0) {
    mtlj.writeMicroseconds(map(news, 1, 99, 1540, 1900));
    Serial.println((String) "left motor speed= " + news + " state " + newt);
    smlj = news;
    tmlj = 1;
  }
  if (newt == 2 && news != 0 && news != 0) {
    mtlj.writeMicroseconds(map(news, 1, 99, 1460, 1100));
    Serial.println((String) "left motor speed= " + news + " state " + newt);
    smlj = news;
    tmlj = 2;
  }
}
//************************************motor R bala
void mru(float news, byte newt) {
  if (news < 0) {
    news = 0;
  }
  if (news > 99) {
    news = 99;
  }

  if (news == 0 || newt == 0) {
    mtru.writeMicroseconds(1500);
    tmru = 0;
    smru = 0;
  }
  if (newt == 1 && news != 0) {
    mtru.writeMicroseconds(map(news, 1, 99, 1540, 1900));
    smru = news;
    tmru = 1;
  }
  if (newt == 2 && news != 0) {
    mtru.writeMicroseconds(map(news, 1, 99, 1460, 1100));
    smru = news;
    tmru = 2;
  }
}
//*****************************************motor L bala
void mlu(float news, byte newt) {
  if (news < 0) {
    news = 0;
  }
  if (news > 99) {
    news = 99;
  }

  if (news == 0 || newt == 0) {
    mtlu.writeMicroseconds(1500);
    tmlu = 0;
    smlu = 0;
  }
  if (newt == 1 && news != 0) {
    mtlu.writeMicroseconds(map(news, 1, 99, 1540, 1900));
    smlu = news;
    tmlu = 1;
  }
  if (newt == 2 && news != 0) {
    mtlu.writeMicroseconds(map(news, 1, 99, 1460, 1100));
    smlu = news;
    tmlu = 2;
  }
}
//*******************************************motor aghab
void ma(float news, byte newt) {
  if (news < 0) {
    news = 0;
  }
  if (news > 99) {
    news = 99;
  }

  if (news == 0 || newt == 0) {
    mta.writeMicroseconds(1500);
    tma = 0;
    sma = 0;
  }
  if (newt == 1 && news != 0) {
    mta.writeMicroseconds(map(news, 1, 99, 1540, 1900));
    sma = news;
    tma = 1;
  }
  if (newt == 2 && news != 0) {
    mta.writeMicroseconds(map(news, 1, 99, 1460, 1100));
    sma = news;
    tma = 2;
  }
}
void led_volume(byte volume) {
  if (volume == 0) {
    analogWrite(led_pin, 0);
    Serial.println("led_off");
  } else {
    analogWrite(led_pin, map(volume, 1, 9, 20, 255));
    Serial.println("led_on");
  }
}
//=================================================================
void orange_led(byte volume) {
  if (volume == 0) {
    analogWrite(led_orange, 0);
    Serial.println("led_off");
  } else {
    analogWrite(led_orange, map(volume, 1, 9, 20, 255));
    Serial.println("led_on");
  }
}
//===================================================================
void white_led(byte volume) {
  if (volume == 0) {
    analogWrite(led_white, 0);
    Serial.println("led w _off");
  } else {
    analogWrite(led_white, map(volume, 1, 9, 20, 255));
    Serial.println("led w _on");
  }
}
//=================================================================
void motor_sport(char in) {
  switch (in) {
    case 'w':
      digitalWrite(p_r1ms, LOW);
      analogWrite(p_r2ms, 155);
      Serial.println("on");
      //      #analogWrite(p_ms, 255);
      break;
    case 's':
      digitalWrite(p_r2ms, LOW);
      analogWrite(p_r1ms, 155);
      Serial.println("on");
      //      #analogWrite(p_ms, 255);
      break;
    case 'n':
      digitalWrite(p_r1ms, LOW);
      digitalWrite(p_r2ms, LOW);
      Serial.println("off");
      //      analogWrite(p_ms, 0);
      break;
  }
}
float get_temp() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}
void zarib_sport(char in1,char in2){
 String tempin = ((String)in1 + in2);
      float val = (tempin.toInt()); 
      zarib_p_sport=val/100;
      Serial.println(zarib_p_sport);

}void zarib_taadol(char in1,char in2){
 String tempin = ((String)in1 + in2);
      float val = (tempin.toInt()); 
      zarib_jelobandi=val/100;
      Serial.println(zarib_jelobandi);

}
