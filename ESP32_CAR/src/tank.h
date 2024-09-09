#include <motors.h>
#include <tank_classes.h>


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

  Servo servo1;
  Servo servo2;
  Servo servo3;
  Servo servo4;


  

  bool STDBY = false; // power consumption mode. False: not in STBY mode (out signal is HIGH)
  bool motors_on = false;




    void go_f_auto() {
        int f_cur_dist;
        int start_speed = START_SPEED;
        Serial.print("in go_f_auto    "); 

        go_forward(start_speed);
        
        while(true) {
            f_cur_dist = f_sensor.read_dist();

            if (f_cur_dist==0)
                f_cur_dist=FAR;
            Serial.print(f_cur_dist); 
            Serial.print("  .  "); 


            if (f_cur_dist>CLOSE){
                go_forward(start_speed);
            }
            else {
                go_left_pivot(start_speed);
            }

        delay(500);
        } // of while loop
    // TBD: write - get input from sensor and turn accordingly
    } // go_auto_F()






  void init_all() {  
    i2c_devs.i2c_init();
    init_motors();
    init_us_sensors();
    // init_servos(); // init servos requires more power from the cpu
    
  } // of init_all()


void init_servos() {
    servo1.attach(F_SERVO_PWM_PIN);
    servo1.attach(B_SERVO_PWM_PIN);
    servo1.attach(R_SERVO_PWM_PIN);
    servo1.attach(L_SERVO_PWM_PIN);
} // of init_servos()




void test_servos() {
    Serial.println(" testing Servos xxx");

    for (int i=0;i<180;i+=20) {
        servo1.write(i);
        servo2.write(i);
        servo3.write(i);
        servo4.write(i);
        Serial.print(i);
        Serial.print(" . ");
        delay(100);
    } // of for loop

    for (int i=180;i>0;i-=20) {
    servo1.write(i);
    servo2.write(i);
    servo3.write(i);
    servo4.write(i);

    Serial.print(i);
    Serial.print(" . ");
    delay(100);
    } // of for loop


} // of test_servosxxx()


  void init_motors() {  
    left_motor.init(M1_IN1_pin,M1_IN2_pin,PWM_Channel0,PWM_Channel1);
    right_motor.init(M2_IN1_pin,M2_IN2_pin,PWM_Channel2,PWM_Channel3);
  }


  void init_us_sensors() {
    //Serial.print(" // in tank_init_us_sensors() //  ");
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

  void go_vector(int _x, int _y,int _button,int _range) {

    // go by vector recieved from joystick;
    Serial.print("  >> in tank_go_vector: x/y/button/range ");
    Serial.print(_x);
    Serial.print("..  ");
    Serial.print(_y);
    Serial.print("..  ");
    Serial.print(_button);
    Serial.print("..  ");
    Serial.print(_range);
    Serial.print("..  ");
    
    
    
    
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
      go_forward(y_speed);      
      return;
    }

    //if (_x==0 && _y<0) {
    if (x_speed==0 && y_speed<0) {
      Serial.println("  backward");
      go_backward(y_speed);
      return;
    }

    //if (_x>0 && _y==0) {
    if (x_speed>0 && y_speed==0) {
      Serial.println("  Right Pivot");
      go_right_pivot(x_speed);
      return;
    }

    //if (_x<0 && _y==0) {
    if (x_speed<0 && y_speed==0) {
      Serial.println("  Left Pivot");
      go_left_pivot(x_speed);
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


  void go_forward(int _speed) {
    left_motor.Go_forward(_speed);
    right_motor.Go_forward(_speed);    
  }


  void go_backward(int _speed) {
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

  void go_left_pivot(int _speed) {
    left_motor.Go_backward(_speed);
    right_motor.Go_forward(_speed);    
  }

  void go_right_pivot(int _speed) {
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
    for (int i = 150;i<255;i+=25) {
        Serial.print(i);
        Serial.print("  .  ");

        go_forward(i);
        delay(5000);
    } // of for loop

    Serial.println("");
    Serial.print("in test_motors: Backward"   );
    for (int i = 150;i<155;i+=25) {
        Serial.print(i);
        Serial.print("  .  ");

        go_backward(i);
        delay(5000);
    } // of for loop

}  // of test motors




void test_sensors() {
  /// parameters: which sensorm num of trials, delay 

  Serial.println("testing Sensor: Front");
  test_sensor(f_sensor,5,200);
    //Serial.println("testing Sensor: Back");
  //test_sensor(b_sensor,10,200);
  //Serial.println("testing Sensor: Right");
  //test_sensor(r_sensor,10,200);
  //Serial.println("testing Sensor: left");
  //test_sensor(l_sensor,10,200);
  } // of test_sensors()

  void test_sensor(US_Sensor _S_Sensor, int _num,int _delay) {
    Serial.println(" .... starting test_sensor()    ");
    for (int i=1;i<=_num;i++) {
      int l_dist = _S_Sensor.read_dist();
      Serial.print(l_dist);
      Serial.print("  .  ");
      delay(_delay);
    }    
  } // of test_sensor()

}; // of TANK class
