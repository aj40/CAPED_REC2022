#ifndef REC_h
#define REC_h

//ADDRESSES
#define OP 0x10 //aka 16
#define Z1 0x11 //aka 17
#define Z2 0x12 //aka 18
#define Z3 0x13 //aka 19
#define TOF1 0x20 //aka 32
#define TOF2 0x21 //aka 33
#define SERVO 0x30 //aka 48

//DATA
#define SET_STOP_F  10
#define SET_STOP_T  11
#define SET_Z1_F    20
#define SET_Z1_T    21
#define SET_Z2_F    30
#define SET_Z2_T    31
#define SET_Z3_F    40
#define SET_Z3_T    41
#define JOG_Q_F     50
#define JOG_Q_T     51
#define JOG_LT_F    60
#define JOG_LT_T    61
#define JOG_BR_F    70
#define JOG_BR_F    71
#define JOG_LT_F    80
#define JOG_LT_T    81

// ServoDriver Definitions:
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#endif

