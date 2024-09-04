#include "WiFi.h"
#include <esp_now.h>
#include<tank_classes.cpp>
//#include <i2c_devices.h>
//#include <Wire.h> // for I2C
//#include "PCF8574.h"


Tank my_tank;
//Motor motor_left; // canceled Oct 2, these are within Tank class
//Motor motor_right;
int rc_x = 0;
int rc_y = 0;
int range=12; // perhaps this should be bessaged from RC, becaue same value here and there



typedef struct struct_message {
  int x_val;
  int y_val;
  bool button_state;
  char ctrl_msg[32];
  //float c;
  //String d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  my_tank.tank_go_vector(myData.x_val,myData.y_val,myData.button_state,range);
} // of OnDataRecv() 

//PCF8574 i2c_ctrl(0x38);

//i2c_devices my_dev;




// ****************** SETUP ************* 
void setup()
{
  Serial.begin(9600);
  Serial.print("");
  Serial.println("in SETUP: Starting");

// Checking I2C
  Wire.begin();
//my_dev.i2c_scanner();

my_tank.i2c_devs.i2c_init();
my_tank.tank_init_motors(M1_IN1_pin,M1_IN2_pin,PWM_Channel0,PWM_Channel1,M2_IN1_pin,M2_IN2_pin,PWM_Channel2,PWM_Channel3);


//my_tank.left_motor.init(M1_IN1_pin,PWM1_pin,PWM_Channel0,PWM_Channel1);
//my_tank.right_motor.init(M2_IN1_pin,PWM2_pin,PWM_Channel2,PWM_Channel3);



while (true) {
  //my_tank.test_moves();
  my_tank.test_motors();
};









while (true) {

  Serial.print(".");
  
  delay(500);
}
  
  Serial.println("here 1");
  WiFi.mode(WIFI_MODE_STA);
  Serial.println("here 2");
  Serial.println(WiFi.macAddress());
  Serial.println("here 3");

  
  int cnt=0;
  while(true) {
    Serial.println(cnt++);
    delay(1000);

  }
  

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("in SETUP: Error initializing ESP-NOW");
    return;
  }
  Serial.println("in SETUP: init ESP-NOW ok");
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

   // pinMode(LED_BUILTIN, OUTPUT);  
//   pinMode(AIN1_pin, OUTPUT);
//   pinMode(AIN2_pin, OUTPUT);
   //pinMode(PWMA_pin, OUTPUT);  // done later as ledcAttachPin
   //pinMode(BIN1_pin, OUTPUT);    // done later as ledcAttachPin
   //pinMode(BIN2_pin, OUTPUT);
   //pinMode(PWMB_pin, OUTPUT);
   //pinMode(Spare_LED, OUTPUT);

   pinMode(F_TRIG_PIN, OUTPUT);
   pinMode(B_TRIG_PIN, OUTPUT);
   pinMode(R_TRIG_PIN, OUTPUT);
   pinMode(L_TRIG_PIN, OUTPUT);
   pinMode(F_ECHO_PIN, INPUT);
   pinMode(B_ECHO_PIN, INPUT);
   pinMode(R_ECHO_PIN, INPUT);
   pinMode(L_ECHO_PIN, INPUT);
   
   //pinMode(STBY_pin, OUTPUT);
   pinMode(LED_MOV_pin, OUTPUT);
   pinMode(BUZZER_pin, OUTPUT);
   

// For the tank DC motors
   // for ESP32 - different than ESp8266/WEMOS
   // example from: https://randomnerdtutorials.com/esp32-pwm-arduino-ide/
   // configure LED PWM functionalitites
   // L_PWM_Channel,R_PWM_Channel are the pwm channels used 

  //ledcSetup(L_PWM_Channel, MOTOR_FREQ, PWM_REOLUTION);
  //ledcSetup(R_PWM_Channel, MOTOR_FREQ, PWM_REOLUTION);
  
  // attach the channel to the GPIO to be controlled
  //ledcAttachPin(PWMA_pin, L_PWM_Channel);
  //ledcAttachPin(PWMB_pin, R_PWM_Channel);
 
  //my_tank.tank_init_motors(AIN1_pin,AIN2_pin,PWMA_pin, L_PWM_Channel, BIN1_pin,BIN2_pin,PWMB_pin, R_PWM_Channel, STBY_pin);
  Serial.println("in SETUP: init motors done");
  //my_tank.tank_init_servos(F_SERVO_PWM_PIN,B_SERVO_PWM_PIN,R_SERVO_PWM_PIN,L_SERVO_PWM_PIN);
  Serial.println("in SETUP: init servos done");

  
  digitalWrite(BUZZER_pin,HIGH); // we are ready to go !
  delay(1000);
  digitalWrite(BUZZER_pin,LOW); // we are ready to go !
  Serial.println("in SETUP: starting test_hw()");

 // my_tank.test_hw();  // 
  Serial.println("in SETUP: finished test_hw()");

  //my_tank.tank_init_us_sensors();

   //my_tank.tank_test();
   
   // my_tank.test_sensor(my_tank.f_sensor,5,200);  // params: num of readings, delay 
  //    Serial.println("testing Sensor: Front");
//   my_tank.test_sensor(my_tank.f_sensor,5,200);  // params: num of readings, delay 

//   my_tank.set_motors_on();
//   my_tank.test_moves();
//   my_tank.set_motors_off();

   digitalWrite(LED_MOV_pin,HIGH); // we are ready to go !
   //my_tank.set_motors_on();
   Serial.println("End of setup");
 
  
} // of SETUP

// ***************** LOOP ************

void test_motors() {
  //int test_speed = 100;
  int tmp_inc = 40;
  int tmp_delay = 2000;
  bool led_flag = false;

  my_tank.tank_go_vector(0,0,0,SERVO_RANGE); // stop both engines
  delay(500);

  Serial.println("Going forward");
  
  for (int y=0;y<SERVO_RANGE;y+=tmp_inc) {
    if (led_flag) {
      led_flag = false;
      my_tank.moving_led.turn_led_on();
    } // of IF 
    else {
      led_flag = true;
      my_tank.moving_led.turn_led_off();
    } // of ELSE
    
    Serial.print("  y= ");
    Serial.print(y);
    my_tank.tank_go_vector(0,y,0,SERVO_RANGE);   
    delay(tmp_delay);
  } // of for() loop
  Serial.println("Going BackWards");

  my_tank.tank_go_vector(0,0,0,SERVO_RANGE); // stop both engines
  delay(500);

  for (int y=0;abs(y)<SERVO_RANGE;y-=tmp_inc) {
      if (led_flag) {
      led_flag = true;
      my_tank.moving_led.turn_led_on();
    }
    else {
      led_flag = false;
      my_tank.moving_led.turn_led_off();
    }


    Serial.print("  y= ");
    Serial.print(y);
    my_tank.tank_go_vector(0,y,0,-SERVO_RANGE);   
    delay(tmp_delay);
  } // of for() loop
  //Serial.println("end of Y cycle, starting X cycle");

} // of test_motors() 

void test_tank() {
  int inc=40;
  Serial.println("FORWARD  ");
  for (int speed=0;speed<SERVO_RANGE;speed+=inc) {
    //Serial.print(speed);
    //Serial.print("  ");
    my_tank.Tank_forward(speed);
    delay(2000);
  } // of for() 

  my_tank.tank_stop();
  Serial.println("");
  Serial.println("BACKWARD");
  for (int speed=0;speed<SERVO_RANGE;speed+=inc) {
    //Serial.print(speed);
    //Serial.print("  ");
    my_tank.Tank_backward(speed);
    delay(2000);
  } // of for()
  my_tank.tank_stop();
  Serial.println("");
  
}


void loop() 
{
     //delay(100);
     //test_motors();
     test_tank();
} // of LOOP