#include "WiFi.h"
#include <esp_now.h>
#include <tank.h>

//#include<tank_classes.h>
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
  my_tank.go_vector(myData.x_val,myData.y_val,myData.button_state,range);
} // of OnDataRecv() 




// ****************** SETUP ************* 
void setup()
{
  Serial.begin(9600);
  Wire.begin(); // start I2C
  Serial.print("");
  Serial.println("in SETUP: Starting");
  
  my_tank.init_all();
  my_tank.i2c_devs.leds_ctrl(true,true,true); // red, yellow, green
  delay(1000);

/*
while(true) {
  my_tank.test_servos();
}
*/

  my_tank.go_f_auto();

  while(true) {
    //my_tank.i2c_devs.test_I2C_devs();
    //my_tank.test_sensors();
    //my_tank.test_servosxxx();
    my_tank.test_motors();
    
  };




  
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

/*
   pinMode(F_TRIG_PIN, OUTPUT);
   pinMode(B_TRIG_PIN, OUTPUT);
   pinMode(R_TRIG_PIN, OUTPUT);
   pinMode(L_TRIG_PIN, OUTPUT);
   pinMode(F_ECHO_PIN, INPUT);
   pinMode(B_ECHO_PIN, INPUT);
   pinMode(R_ECHO_PIN, INPUT);
   pinMode(L_ECHO_PIN, INPUT);
   */
   
   //pinMode(STBY_pin, OUTPUT);
   pinMode(LED_MOV_pin, OUTPUT);
   pinMode(BUZZER_pin, OUTPUT);
   

  
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

  my_tank.go_vector(0,0,0,SERVO_RANGE); // stop both engines
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
    my_tank.go_vector(0,y,0,SERVO_RANGE);   
    delay(tmp_delay);
  } // of for() loop
  Serial.println("Going BackWards");

  my_tank.go_vector(0,0,0,SERVO_RANGE); // stop both engines
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
    my_tank.go_vector(0,y,0,-SERVO_RANGE);   
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
    my_tank.go_forward(speed);
    delay(2000);
  } // of for() 

  my_tank.tank_stop();
  Serial.println("");
  Serial.println("BACKWARD");
  for (int speed=0;speed<SERVO_RANGE;speed+=inc) {
    //Serial.print(speed);
    //Serial.print("  ");
    my_tank.go_backward(speed);
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