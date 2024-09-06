//#include <tank_headers.h>
#include <motors.h>

#include "WiFi.h"
//#include <ESP32Servo.h>
//#include <NewPing.h>
//#include<ESP8266WiFi.h>
#include <i2c_devices.h>
#include <ESP32Servo.h>




// no need for this class? there is a servo class builtin
class us_Servo {
  public:
  int s_channel = 77;
  int s_pin = 777; 
  int s_freq = 77777;
  int s_resolution = 777777;
  

  void init(int _channel, int _pin, int _freq, int _resolution) {
    s_channel = _channel;
    s_pin = _pin;
    s_freq = _freq;
    s_resolution = _resolution;

    ledcSetup(s_channel, s_freq, s_resolution);
    ledcAttachPin(s_pin, s_channel);
  }

  void write_angle(int _angle) {
      int val = map(_angle,0,180,0,255);
     ledcWrite(s_channel, val);
     Serial.println("");
     Serial.print("  Angle: ");
     Serial.print(_angle);
     Serial.print("  value: ");
     Serial.print(val);
    Serial.print("  Channel: ");
     Serial.print(s_channel);
     Serial.print("  Pin: ");
    Serial.print(s_pin);
    Serial.print("  Freq: ");
    Serial.print(s_freq);
    Serial.print("  resolution: ");
     Serial.println(s_resolution);
     
    
  }

  void test_servo() {
    Serial.println("testing Servo");
    Serial.print("   Channel: ");
    Serial.print(s_channel);
    Serial.print("   pin: ");
    Serial.print(s_pin);
    Serial.print("   freq: ");
    Serial.print(s_freq);
    Serial.print("   Resolution: ");
    Serial.println(s_resolution);

  Serial.println("Angle:");
    
    
  
    for (int angle=10;angle<180;angle+=20) {
      write_angle(angle);
      //Serial.print(angle);
      //Serial.print(" . ");
      delay(1000);
    } // of for loop

    for (int angle=170;angle>0;angle-=20) {
      write_angle(angle);
      //Serial.print(angle);
      //Serial.print(" . ");
      delay(1000);
    } // of for loop
  }

  
}; // of class us_servo


class US_Sensor {
  public:
  int trig_pin;
  int echo_pin;

  void dum() {
  } // of dum()

  void init(int _trig, int _echo) {
    trig_pin = _trig;
    echo_pin = _echo;
  } 

  int read_dist() {
    float duration_us, distance_cm;

    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);

    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trig_pin, LOW);
    duration_us  = pulseIn(echo_pin, HIGH);
     distance_cm = 0.017 * duration_us;

    return(distance_cm);
  }
}; // of Sensor class

class led {
  public:

  void turn_led_on() {
    digitalWrite(LED_MOV_pin,HIGH);
  } // of turn_led_on()
   
    void turn_led_off() {
    digitalWrite(LED_MOV_pin,LOW);
  } // turn_led_off()
}; // of LED class



class Tank {
  public:
  Motor left_motor;
  Motor right_motor;
  led moving_led;
  // 2 Sep 24
  i2c_devices i2c_devs;

    

  
  US_Sensor f_sensor;
  US_Sensor b_sensor;
  US_Sensor r_sensor;
  US_Sensor l_sensor;

  us_Servo f_servo; 
  us_Servo b_servo; 
  us_Servo r_servo; 
  us_Servo l_servo; 
  

  bool STDBY = false; // power consumption mode. False: not in STBY mode (out signal is HIGH)
  bool motors_on = false;

  void tank_init_motors(int _in1_pin,int _in2_pin,int _chan_a,int _chan_b,int _in3_pin,int _in4_pin ,int _chan_c,int _chan_d) {  
    left_motor.init(_in1_pin,_in2_pin,_chan_a,_chan_b);
    right_motor.init(_in3_pin,_in4_pin,_chan_c,_chan_d);
  }

   void tank_init_servos() {
    f_servo.init(F_Servo_PWM_Channel,F_SERVO_PWM_PIN,SERVO_FREQ,PWM_RESOLUTION);
    b_servo.init(B_Servo_PWM_Channel,B_SERVO_PWM_PIN,SERVO_FREQ,PWM_RESOLUTION);
    r_servo.init(R_Servo_PWM_Channel,R_SERVO_PWM_PIN,SERVO_FREQ,PWM_RESOLUTION);
    l_servo.init(L_Servo_PWM_Channel,L_SERVO_PWM_PIN,SERVO_FREQ,PWM_RESOLUTION);
  } // of tank_init_servos()

  void tank_init_us_sensors() {
    f_sensor.init(F_TRIG_PIN,F_ECHO_PIN);
    b_sensor.init(B_TRIG_PIN,B_ECHO_PIN);
    r_sensor.init(R_TRIG_PIN,R_ECHO_PIN);
    l_sensor.init(L_TRIG_PIN,L_ECHO_PIN);
  }



// XXXXXXXXXXXXX
  void tank_stop() {
    left_motor.stop();
    right_motor.stop();
  }

  void tank_go_vector(int _x, int _y,int _button,int _range) {

    Serial.print("  >> in tank_go_vector: x/y/button/range ");
    Serial.print(_x);
    Serial.print("..  ");
    Serial.print(_y);
    Serial.print("..  ");
    Serial.print(_button);
    Serial.print("..  ");
    Serial.print(_range);
    Serial.print("..  ");
    
    // go by vector recieved from joystick;
    
    
    int x_speed = abs(_x);
    int y_speed = abs(_y);

    x_speed = map(x_speed,0,_range,0,SERVO_RANGE); // 255 for 8 bits
    y_speed = map(y_speed,0,_range,0,SERVO_RANGE);

    Serial.print("   x_speed / y_speed: x/y ");
    Serial.print(x_speed);
    Serial.print("..");
    Serial.print(y_speed);

    //if (_x==0 && _y==0) {
    if (x_speed==0 && y_speed==0) {
      Serial.println("  stop");
      left_motor.stop();
      right_motor.stop();
      return;
    }

    //if (_x==0 && _y>0) {
    if (x_speed==0 && y_speed>0) {
      Serial.println("  forward");
      Tank_forward(y_speed);      
      return;
    }

    //if (_x==0 && _y<0) {
    if (x_speed==0 && y_speed<0) {
      Serial.println("  backward");
      Tank_backward(y_speed);
      return;
    }

    //if (_x>0 && _y==0) {
    if (x_speed>0 && y_speed==0) {
      Serial.println("  Right Pivot");
      Tank_right_pivot(x_speed);
      return;
    }

    //if (_x<0 && _y==0) {
    if (x_speed<0 && y_speed==0) {
      Serial.println("  Left Pivot");
      Tank_left_pivot(x_speed);
      return;
    }

  // now deal with vector moves
    if (_x>0 && _y>0) {
      // FWD R
      int r_speed = map(x_speed,0,SERVO_RANGE,y_speed,0);  

      Serial.print("  FWD R ");
      Serial.print(x_speed);
      Serial.print(" , ");
      Serial.print(y_speed);
      Serial.println(" ");
      
      // go FWD right
      Tank_forward_turn(y_speed,r_speed);
      return;
    }

      if (_x<0 && _y>0) {
        // FWD L
        int l_speed = map(x_speed,0,SERVO_RANGE,y_speed,0);  
        Serial.print("  FWD L ");
        Serial.print(x_speed);
        Serial.print(" , ");
        Serial.print(y_speed);
        Serial.print(" , r_speed: ");
        Serial.print(l_speed);
        Serial.println(" ");
      // go FWD left
      
      Tank_forward_turn(y_speed,l_speed);
      return;
    }

    if (_x>0 && _y<0) {
      // BWD R
      int r_speed = map(x_speed,0,SERVO_RANGE,y_speed,0);  
      Serial.println("  BWD R ");      
      Serial.print(x_speed);
      Serial.print(" , ");
      Serial.print(y_speed);
      Serial.println(" ");
      // go BWD right
      Tank_backward_turn(y_speed,r_speed);
      return;
    }

    if (_x<0 && _y<0) {
      // BWD L
      int l_speed = map(x_speed,0,SERVO_RANGE,y_speed,0); 
      Serial.print("  BWD L ");
      Serial.print(x_speed);
      Serial.print(" , ");
      Serial.print(y_speed);
      Serial.println(" ");
      // go BWD left
      Tank_backward_turn(l_speed,y_speed);
      return;
    }

  Serial.println("  should NOT be here");

  } // of tank_go_vector


  void Tank_forward(int _speed) {
    left_motor.Go_forward(_speed);
    right_motor.Go_forward(_speed);    
  }


  void Tank_backward(int _speed) {
      left_motor.Go_backward(_speed);
      right_motor.Go_backward(_speed);    
    }

  void Tank_forward_turn(int _l_speed,int _r_speed) {
    left_motor.Go_forward(_l_speed);
    right_motor.Go_forward(_r_speed);    
  }  

  void Tank_backward_turn(int _l_speed,int _r_speed) {
    left_motor.Go_backward(_l_speed);
    right_motor.Go_backward(_r_speed);    
  }

  void Tank_left_pivot(int _speed) {
    left_motor.Go_backward(_speed);
    right_motor.Go_forward(_speed);    
  }

  void Tank_right_pivot(int _speed) {
    left_motor.Go_forward(_speed);
    right_motor.Go_backward(_speed);    
  }

  void tank_move(int _x,int _y) {    

      if (0 == _x && 0 == _y) {
        tank_stop();
        Serial.println("stopping  ");
        return;
      }

    
      int ratio;
      if (0 == _y || 0 == _x)
        ratio=1;      
      else
        ratio = abs(int(_x/_y));

/*
      Serial.print("x / y / ratio : ");
      Serial.print(_x);
      Serial.print("  ");
      Serial.print(_y);
      Serial.print("  ");
      Serial.println(ratio);
*/      


      if (_x>=0 && _y>0) {
        // 1Q
        // forward and right, right motor slowing
        // if x=0 it is fully FWD
        left_motor.Go_forward(_y);
        right_motor.Go_forward(abs(_y*ratio));
        Serial.println(" 1Q ");
        return;
      }
      if (_x>=0 && _y<0) {
        // 2Q
        // backward and left, left engine slowing
        left_motor.Go_backward(abs(_y));
        right_motor.Go_backward(abs(_y*ratio));
        Serial.println(" 2Q ");
        return;
      }
      if (_x<=0 && _y<0) {
        // 3Q
        // bacward and right, right engine slowing
        left_motor.Go_backward(abs(_y));
        right_motor.Go_backward(abs(_y*ratio));
        Serial.println(" 3Q ");
        return;
      }
      if (_x<0 && _y>0) {
        // 4Q
        // forward and left, left motor slowing
        left_motor.Go_forward(abs(int(_y*ratio)));
        right_motor.Go_forward(abs(_y));
        Serial.println(" 4Q ");
        return;
      }

      if (_y==0 && _x>0 ) {
        // pivot Right
        left_motor.Go_forward(_x);
        right_motor.Go_backward(_x);
        return;
      }

      if (_y==0 && _x<0 ) {
        // pivot Left
        left_motor.Go_backward(abs(_x));
        right_motor.Go_forward(abs(_x));
        return;
      }

      Serial.println("ERROR - should not be here, at TANK_MOVE");


   }  // end of tank_move

void test_motors() {
    Serial.println("");
    Serial.print("in test_motors: Forward"   );
    for (int i = min_PWM;i<255;i+=25) {
        Serial.print(i);
        Serial.print("  .  ");

        Tank_forward(i);
        delay(1000);
    } // of for loop

    Serial.println("");
    Serial.print("in test_motors: Backward"   );
    for (int i = min_PWM;i<255;i+=25) {
        Serial.print(i);
        Serial.print("  .  ");

        Tank_backward(i);
        delay(1000);
    } // of for loop

}  // of test motors


void test_moves() {
  int test_speed = 100;
  int l_speed = 100;
  int r_speed = 100;
  int test_time = 1000;
  int stop_time = 500;
  
  
  // test Forward
  Serial.println("in test_moves: Forward");
  Tank_forward(test_speed);
  delay(test_time);
  tank_stop();
  delay(stop_time);

  // test Backward
  Serial.println("in test_moves: Backward");
  Tank_backward(test_speed);
  delay(test_time);
  tank_stop();
  delay(stop_time);

  // test Left turn
  r_speed = 200;
  Serial.println("in test_moves: Left turn");
  Tank_forward_turn(l_speed,r_speed);
  delay(test_time);
  tank_stop();
  delay(stop_time);

  // test Right turn
  r_speed = 100;
  l_speed = 200;
  Serial.println("in test_moves: Right turn");
  Tank_forward_turn(l_speed,r_speed);
  delay(test_time);
  tank_stop();
  delay(stop_time);

  // test right pivot
  Serial.println("in test_moves: Right Pivot");
  Tank_right_pivot(test_speed);
  delay(test_time);
  tank_stop();
  delay(stop_time);

  // test left pivot
  Serial.println("in test_moves: Left Pivot");
  Tank_left_pivot(test_speed);
  delay(test_time);
  tank_stop();
  delay(stop_time);

} // of test_moves

// basic HW test, to make sure pins don;t create issues
// to delete when all works fine
void test_hw() {
  for (int i=1;i<4;i++) {
    digitalWrite(LED_MOV_pin,HIGH);
    delay(500);  
    digitalWrite(LED_MOV_pin,LOW);
    delay(400);  
  } // of test_hw()

  test_servos();
  test_moves();
} // of test_hw()

void test_servos() {
  //test_servos();
  Serial.print("testing Servo: Front: ");
  return;
  test_servo(f_servo);
  return;
  Serial.println("testing Servo: Back");
  test_servo(b_servo);
  Serial.println("testing Servo: Right");
  test_servo(r_servo);
  Serial.println("testing Servo: Left");
  test_servo(l_servo);
} // of test_servos()

void test_servo(us_Servo _servo_name) {
  for(int i = 3; i <= 30; i += 2) {
    Serial.print(i);
    Serial.print("___");
    _servo_name.write_angle(i);
    delay(500);
    _servo_name.write_angle(ZERO);
    delay(500);
    _servo_name.write_angle(15);
  } // of for loop
    
    
    //_servo_name.write_angle(ZERO);
    //delay(100); // move from 180 to 0 degrees with a negative angle of 5 for(angle = 180; angle>=1; angle-=5)
} // of test_servo()


  void test_sensors() {
     Serial.println("testing Sensor: Front");
  test_sensor(f_sensor,5,200);
  Serial.println("testing Sensor: Back");
  test_sensor(b_sensor,10,200);
  Serial.println("testing Sensor: Right");
  test_sensor(r_sensor,10,200);
  Serial.println("testing Sensor: left");
  test_sensor(l_sensor,10,200);
  } // of test_sensors()

  void test_sensor(US_Sensor _S_Sensor, int _num,int _delay) {
    for (int i=1;i<=_num;i++) {
      int l_dist = _S_Sensor.read_dist();
      Serial.print("Sensor reading: ");
      Serial.println(l_dist);
      delay(_delay);
    }
    
  } // of test_sensor()


  void  tank_test() {
    Serial.println("test tank moves: FW/BW/R/L/R pivot/L pivot");
    //set_motors_on();
    test_moves();
    //set_motors_off();
    delay(1000);

    test_sensors();
    test_servos();
    return;
  } // of tank_test()


}; // of TANK class