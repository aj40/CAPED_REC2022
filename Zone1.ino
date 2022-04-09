// ZONE1 Code
// by AJ 4/3/22

#include <Wire.h>
#include "REC.h"
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_VL53L0X.h>

//Definitions
#define IR_PREQ 1
#define IR_Q    2
#define IR_B1   3
#define IR_B2   4
#define LIMIT_B 5
#define STEP    6
#define DIR     7
#define IR_T1   8
#define IR_T2   9
#define IR_BR   10

//Servo Defs
#define Q_D     1
#define CABIN_D 2
#define CABIN_L 3

//Create Booleans
boolean done = false;
boolean z2 = true;
boolean z3 = true;
boolean jogQ = false;
boolean jogLT = false;

//Create Servo Driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//Create TOF
Adafruit_VL53L0X tof = Adafruit_VL53L0X();

void setup() {
  // put your setup code here, to run once:
  pinMode(IR_PREQ, INPUT);
  pinMode(IR_Q, INPUT);
  pinMode(IR_B1, INPUT);

  Wire.begin(Z1);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.begin(9600);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  tof.begin();

  delay(10);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  tof.rangingTest(&measure, false);

  // put your main code here, to run repeatedly:
  if(!jogQ && z3 && digitalRead(IR_PREQ)){
    done = false; //resets everytime something enters prequeue
    //while not in queue position
    while(!digitalRead(IR_Q)){
      pwm.setPWM(Q_D, 0, 350); //half speed clockwise
    }
    pwm.setPWM(Q_D, 0, 300); //stationary

    delay(9000);
  }

  if(!jogQ && digitalRead(IR_Q) && digitalRead(LIMIT_B) 
      && digitalRead(IR_B1) && digitalRead(IR_B2)){
        
    while(measure.RangeStatus != 4 && measure.RangeMilliMeter < 85){  //While on approach
      pwm.setPWM(Q_D, 0, 400); //max speed clockwise
      pwm.setPWM(CABIN_D, 0, 350);
    }
    pwm.setPWM(Q_D, 0, 300);
    pwm.setPWM(CABIN_D, 0, 300);

    pwm.setPWM(CABIN_L, 0, SERVOMAX);
  }

  if(!jogLT && IR_T1 && IR_T2){
     pwm.setPWM(CABIN_L, 0, SERVOMIN);
     delay(100);
     pwm.setPWM(CABIN_D, 1, 400);
     
  }

}

void receiveEvent(int howMany){
  int d = Wire.read();
  switch(d){
    case SET_STOP_F : jogQ = jogT = false;
                      break;
    case SET_STOP_T : jogQ = jogT = true;
                      break;
    case SET_Z2_F   : z2 = false;
                      break;
    case SET_Z2_T   : z2 = true;
                      break;
    case SET_Z3_F   : z3 = false;
                      break;
    case SET_Z3_T   : z3 = true;
                      break;
    case JOG_Q_F    : jogQ = false;
                      break;
    case JOG_Q_T    : jogQ = true;
                      break;
    case JOG_LT_F   : jogT = false;
                      break;
    case JOG_LT_T   : jogT = true;
                      break;
    default         : Serial.print("Error! Invalid data received at Z1");
  }
}

void requestEvent(){
  if(done){
    Wire.write(SET_Z1_T);
  }
  else{
    Wire.write(SET_Z1_F);
  }
}
