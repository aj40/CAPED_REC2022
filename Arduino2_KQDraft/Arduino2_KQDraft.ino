// This is ARDUINO 2 in the CAPED REC 2022 Multithreaded control system. This ARDUINO has domain over:
//      --Transition from lift to drop across bridge
//      --Engaging Drop Tower lock
//      --Drop tower drop to bottom and rise to top
//      --Actuate drop tower

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_VL53L0X.h>
#include "REC.h" // This will eventually have all the case defintitions and i2c definitions hopefully ServoDriver too


// Sensor Pins
#define DROPTOPIR1 15
#define DROPTOPIR2 16
#define DROPBOTTOMIR1 5
#define DROPBOTTOMIR2 6
#define DROPBOTTOMLIMIT1 2
#define DROPBOTTOMLIMIT2 3
#define DROPTOPLIMIT1 4

// Actuator Info
#define BRIDGESERVOS 7 // Number on the driver board for the bridge servos
#define PREQUEUESERVO 6 // Number on the driver board for the lift tower servos
#define ACTUATESERVO 2 // Number on the driver board for the tilt actuator servo
#define DROPLOCKSERVO 3 // Number on the driver board for the drop tower lock servo
#define DRIVEPWM 4096 //Right now this is a MAX we should tune it to get the speed we want

// ServoDriver Definitions:
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// servo # counter
uint8_t servonum = 0;

// Time of Flight Sensor Measurement
float TOFmeasure = 0.0;

// TOF Reading for NO Vehicles in Zone
float TOFBlank = 0.0; //------------------------UPDATE ME!

//TOF Reading for Vehicle fully in Tower
float TOFDrop = 0.0; //------------------------UPDATE ME!

// Booleans for task control
boolean done = false;
boolean z1 = false;
boolean z3 = false;
boolean jogDT = false;
boolean jogQ = false;

// Some booleans for one shotting
boolean intower = true;
boolean drivetires = true;


// Driver Objects
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(SERVO);    
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (! Serial) {
    delay(1);
  }

  // Initialize the Sensor Pins
  pinMode(DROPTOPIR1, INPUT)
  pinMode(DROPTOPIR2, INPUT)
  pinMode(DROPBOTTOMIR1, INPUT)
  pinMode(DROPBOTTOMIR2, INPUT)
  pinMode(DROPBOTTOMLIMIT1, INPUT)
  pinMode(DROPBOTTOMLIMIT2, INPUT)
  pinMode(DROPTOPLIMIT1, INPUT)
  
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
}

void loop() {
  // Update the TOF reading each loop
  TOFmeasure = measure.RangeMilliMeter;

  if(TOFmeasure <= TOFTower  && digitalRead(DROPTOPIR1) && digitalRead(DROPTOPIR2) && !rideStop && !jog && drivetires) {
    // Request Info from Z1 to confirm cleared
    Wire.requestFrom(Z1, 8, true)

    // Break if Z1 disagrees
    if(!z1) {
      break;
    }
    
    // Declare Zone 2 Active
    done = false;
    
    // Spin Bridge Servos and lift tower servos
    pwm.setPWM(BRIDGESERVOS, DRIVEPWM, 0);
    pwm.setPWM(LIFTSERVO, DRIVEPWM, 0);

    drivetires = false;

  }

  if(TOFmeasure <= TOFDrop+cal_range && TOFmeasure >= TOFTower-cal_range) {
    // Reset previous state
    drivetires = true;
    // Disable drive tires
    pwm.setPWM(BRIDGESERVOS, 0, 4096);
    pwm.setPWM(LIFTSERVO, 0, 4096);
    
    // Lock tower and drop
    pwm.setPWM(DROPLOCKSERVO, 4096, 0);

  }

    // Actuate the rotating mechanism
    pwm.setPWM(ACTUATESERVO, 4096,0);


  if(digitalRead(DROPBOTTOMIR1) && digitalRead(DROPBOTTOMIR2)) {
    // Release the lock
    pwm.setPWM(DROPLOCKSERVO,0,4096);

    // Unactuate
    pwm.setPWM(ACTUATESERVO, 0, 4096);

  // INSERT CODE TO DRIVE PREQUEUE TIRES BASED ON IF QUEUE IS OCCUPIED
    
    // Send that Z2 is complete
    done = true;

    // Publish Z2 as complete to Z1
    Wire.beginTransmission(Z1);
    Wire.write(done);
    Wire.endTransmission();
  }


  }

}

void receiveEvent(int howMany) {
 int d = Wire.read();
  switch(d){
    case SET_STOP_F : jogDT = jogQ = false;
                      break;
    case SET_STOP_T : jogDT = jogQ = true;
                      break;
    case SET_Z1_F   : z1 = false;
                      break;
    case SET_Z1_T   : z1 = true;
                      break;
    case SET_Z3_F   : z3 = false;
                      break;
    case SET_Z3_T   : z3 = true;
                      break;
    case JOG_Q_F    : jogQ = false;
                      break;
    case JOG_Q_T    : jogQ = true;
                      break;
    case JOG_DT_F   : jogDT = false;
                      break;
    case JOG_DT_T   : jogDT = true;
                      break;
    default         : Serial.print("Error! Invalid data received at Z2");
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
