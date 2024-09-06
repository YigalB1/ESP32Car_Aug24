
// motors control pins
// each motor gets 2 pins: one stable (also controlled by PWM),one via PAM
#define M1_IN1_pin   18 
#define M1_IN2_pin   5
#define M2_IN1_pin   23 
#define M2_IN2_pin   19 

const int PWM_Channel0 = 0;
const int PWM_Channel1 = 1;
const int PWM_Channel2 = 2;
const int PWM_Channel3 = 3;
const int F_Servo_PWM_Channel = 4;
const int B_Servo_PWM_Channel = 5;
const int R_Servo_PWM_Channel = 6;
const int L_Servo_PWM_Channel = 7;

#define LED_MOV_pin  15 // D4 , when the tank is moving, on board LED
//#define Spare_LED    15 // D8, currently not in use
#define BUZZER_pin  19 


#define F_TRIG_PIN  27
#define F_ECHO_PIN  34
#define B_TRIG_PIN  12
#define B_ECHO_PIN  13
#define R_TRIG_PIN   2
#define R_ECHO_PIN  14
#define L_TRIG_PIN  15
#define L_ECHO_PIN  35

#define F_SERVO_PWM_PIN 32
#define B_SERVO_PWM_PIN 33
#define R_SERVO_PWM_PIN 25
#define L_SERVO_PWM_PIN 26

// for Servo motors
//const int F_SERVO_Channel = 2;
//const int B_SERVO_Channel = 3;
//const int R_SERVO_Channel = 4;
//const int L_SERVO_Channel = 5;


//const int trig1 = 32;

const int JUNK_VAL = 7777;
const int DIST_BUFF_SIZE = 20;
const int VERY_CLOSE = 23;
const int CLOSE = 23;
const int IN_RANGE = 40;
const int MIN_SPEED = 120; // was 120
const int MAX_SPEED = 1023; // MAX is 1023
const int SPEED_INC = 50;
const int TIME_IN_STATION = 1500;
const int SAMPLE_TIME = 200; // time for loop to wait between each cycle
const int MAX_DISTANCE = 99;


const int LEFT  = 0;
const int RIGHT = 1;
const int FORWARD = 0;
const int BACKARD = 0;

// ESP32 requires channels and ledc write for PWM control

const int MOTOR_FREQ = 5000;
const int SERVO_FREQ = 50;

const int PWM_RESOLUTION = 8; // 8 bits is enough (256 steps)
const int min_PWM = 0;
//int max_PWM = pow(2, PWM_RESOLUTION)-1; // for automation of bits num resolution

const int SERVO_RANGE = 255; // pow(2,PWM_REOLUTION); // 255 ; // because 8 bit 






const int ZERO  = 0;
//const int LEFT_DIR  = 1; // it should have been like LEFT & RIGHT, but sto match the  real engine direction.
//const int RIGHT_DIR = 0;

const bool fixed_speed = false; // True: speed is fixed or stopped. False: speed changes (slower or faster)
const bool DEBUG_MODE = true; // when true, printing debug statemens