// This is ARDUINO 2 in the CAPED REC 2022 Multithreaded control system. This ARDUINO has domain over:
//      --Transition from lift to drop across bridge
//      --Engaging Drop Tower lock
//      --Drop tower drop to bottom and rise to top
//      --Actuate drop tower

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_VL53L0X.h>
#include "BasicStepperDriver.h"
#include "AlexHeader.h" // This will eventually have all the case defintitions and i2c definitions hopefully ServoDriver too



// Stepper Motor Definitions
#define MOTOR_STEPS 200  // 200 steps per revolution--double check against stepper motors (1.8 degrees/step)
#define RPM 20

// Microstepping--We probably won't use this but it will be here just in case
#define MICROSTEPS 1

// Pins for stepper motor driver
#define DIR 8
#define STEP 9
#define SLEEP 13  // An enable pin! Useful for holding position

// ServoDriver Definitions:
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// servo # counter
uint8_t servonum = 0;

// Stepper Motor locating. steps is the maximum range counted from by going from top to bottom
uint16_t steps = 0;

// Time of Flight Sensor Measurement
float TOFmeasure = 0.0;

// TOF Reading for NO Vehicles in Zone
float TOFBlank = 0.0; //------------------------UPDATE ME!

//TOF Reading for Vehicle fully in Tower
float TOFTower = 0.0; //------------------------UPDATE ME!

//TOF Reading for Vehicle waiting in Queue
float TOFQueue = 0.0; //------------------------UPDATE ME!

// Booleans for task control
boolean done = false;
boolean z1 = false;
boolean z3 = false;
boolean jog = false;


// Driver Objects
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(SERVO);    
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (! Serial) {
    delay(1);
  }
  
  Wire.begin(Z2);     // Join I2C Bus with ZONE2 Address
  // Register the appropriate events for receiving and requesting
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.println("I2C Initialized")

  // Start servo driver baord
  pwm.begin();

  // The below functions are calibration functions for the servodriver. If there are issues with the servo, check here and adjsut values
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Standard for analog servos, usually

  Serial.println("PWM Servo Driver Initialized")

  lox.begin(TOF2);   // Initialize for communication with 2nd TOF (the one on the bridge)

  delay(10);

  // Stepper initialization
  stepper.begin(RPM, MICROSTEPS);
  stepper.enable();

  // Below we locate and calibrate the stepper motor

  while(!digitalRead(TOPLIMIT1) && !digitalRead(TOPLIMIT2)) {
    stepper.rotate(1);
    delay(10)
    }

  while (!digitalRead(BOTTOMLIMIT)) {
    stepper.rotate(1);
    steps++;
    delay(10);
  }

  while (!digitalRead(BOTTOMIR1) && !digitalRead(BOTTOMIR2)) {
    stepper.rotate(1);
    delay(10);
  }
}

void loop() {
  // Update the TOF reading each loop
  TOFmeasure = measure.RangeMilliMeter;

  if(TOFmeasure <= TOFTower && TOFmeasure >= TOFQueue && digitalRead(BOTTOMIR1) && digitalRead(BOTTOMIR2) !rideStop && !jog && z1) {
    // Code here to spin drive tires
  }

  

  

  

}

void receiveEvent(int howMany) {
 int d = Wire.read();
  switch(d){
    case SET_STOP_F : jog = false;
                      break;
    case SET_STOP_T : jog = true;
                      break;
    case SET_Z1_F   : z1 = false;
                      break;
    case SET_Z1_T   : z1 = true;
                      break;
    case SET_Z3_F   : z3 = false;
                      break;
    case SET_Z3_T   : z3 = true;
                      break;
    default         : Serial.print("Error! Invalid data received at Z1");
  }
}
void requestEvent(){
  if(done){
    Wire.write(SET_Z2_T);
  }
  else{
    Wire.write(SET_Z2_F);
  }
}
