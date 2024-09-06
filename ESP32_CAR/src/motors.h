#include <tank_headers.h>

// ______________________ class MOTOR ___________________________
class Motor {
  public:
  int motor_en1;
  // int motor_en2; with TA6586 only one EN pin is needed (and PWM)
  //int motor_pwm;    // for ESP8266 style
  
  int speed = 0; // in work, to use as the speed of the train instead of global variable
  int direction = FORWARD;
  int distance = 0;
  // 2 Sep 24
  int pwm_channelA; // each motor has 2 channels, because 2 PWM signals with differentduty cycles 
  int pwm_channelB;  
  int motor_pwm1_pin;  // each motor has 2 control pins, both PWM 
  int motor_pwm2_pin;   // one is "real" pwm (change speed), the other fixed to 0 or 100%
   
  
  void init(int _pwm_pin1,  int _pwm_pin2, int _channel1, int _channel2) {
    // Sep 2024: Since the PWM pin is changing between directions, 
    // I wlll try using both as PWM. One at 100% (so fixed) and the other cganging speed
    // _pwm for ESp8266 style
    // _pwm_channel for esp32 style
    motor_pwm1_pin = _pwm_pin1;
    motor_pwm2_pin = _pwm_pin2;
    pwm_channelA = _channel1;
    pwm_channelB = _channel2;

    ledcSetup(pwm_channelA, MOTOR_FREQ, PWM_RESOLUTION);
    ledcAttachPin(motor_pwm1_pin, pwm_channelA);
    ledcSetup(pwm_channelB, MOTOR_FREQ, PWM_RESOLUTION);
    ledcAttachPin(motor_pwm2_pin, pwm_channelB);
   
    speed=0;
  } // of INIT routine

  void test_motor() {
    Serial.println("testing motor forward");
    for (int i = min_PWM;i<255;i+=25) {
        Serial.println(i);
        Serial.println(" . ");
        ledcWrite(pwm_channelA, i);
        ledcWrite(pwm_channelB, min_PWM);
        
        delay(500);
    } // of for loop
    ledcWrite(pwm_channelA, min_PWM);
    delay(1000);
    Serial.println("testing motor Backwards");
    for (int i = min_PWM;i<255;i+=25) {
        Serial.println(i);
        Serial.println(" . ");
        ledcWrite(pwm_channelA, min_PWM);
        ledcWrite(pwm_channelB, i);
        
        delay(500);
    } // of for loop
    ledcWrite(pwm_channelB, min_PWM);
    Serial.println("testing motor Backwards");

  } // of test_motor()


  void Go_forward ( int _speed_) {
    Serial.println("");
    Serial.print("---->  In go FW   _speed_:");
    Serial.print(_speed_);
    Serial.print("   pwm_channels: ");
    Serial.println(pwm_channelA,pwm_channelB);

    speed = _speed_; // set the class global value

    ledcWrite(pwm_channelA, speed);
    ledcWrite(pwm_channelB, speed);
  } // of Go_forward


  void Go_backward ( int _speed_) {
    Serial.println("");
    Serial.print("---->  In go BW   _speed_:");
    Serial.print(_speed_);
    Serial.print("   pwm_channels: ");
    Serial.println(pwm_channelA,pwm_channelB);

    speed = _speed_; // set the class global value

    ledcWrite(pwm_channelA, speed);
    ledcWrite(pwm_channelB, speed);
  } // of Go_backward()



    void Go_left(int _l_speed, int _r_speed) {
        ledcWrite(pwm_channelA, _l_speed);
        ledcWrite(pwm_channelB, _r_speed);
  }

    void Go_right(int _l_speed, int _r_speed) {
        ledcWrite(pwm_channelA, _l_speed);
        ledcWrite(pwm_channelB, _r_speed);
  }

  void Go_pivot_left(int _speed) {
        ledcWrite(pwm_channelA, _speed);
        ledcWrite(pwm_channelB, min_PWM);
  }

  void Go_pivot_right(int _speed) {
        ledcWrite(pwm_channelA, min_PWM);
        ledcWrite(pwm_channelB, _speed);
  }
    
  void stop () {
      speed = ZERO;
      //analogWrite(motor_pwm,  ZERO);
      ledcWrite(pwm_channelA, min_PWM);
      ledcWrite(pwm_channelB, min_PWM);
      
    } // of STOP routine


    // ****************** increase_speed **********************
    void increase_speed() {

      if (fixed_speed) {
        speed = MAX_SPEED;
        return; // speed is not changing
      }
        
      // speed is not fixed
      speed += SPEED_INC;
      if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
      }
        
    }
    // ****************** decrease_speed **********************
    void decrease_speed() {
      if (fixed_speed) {
        speed = MAX_SPEED;
        return; // speed is not changing
      }
        

      speed -= SPEED_INC;
      if (speed < MIN_SPEED) {
        speed = MIN_SPEED;
      }
        
    }

// ****************** SLOW_DOWN **********************
void slow_down() {
  if (fixed_speed)
    return; // speed is not changing
  speed = MIN_SPEED;
} // of SLOW DOWN

};  // of Motor class
