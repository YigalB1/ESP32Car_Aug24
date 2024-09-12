//#include <tank_headers.h>
//#include <motors.h>

#include "WiFi.h"
//#include <ESP32Servo.h>
//#include <NewPing.h>
#include <i2c_devices.h>
#include <ESP32Servo.h>




// no need for this class? there is a servo class builtin


class US_Sensor {
  public:
  int trig_pin;
  int echo_pin;

  void dum() {
  } // of dum()

  void init(int _trig, int _echo) {
    //Serial.println("US init . Trig/echo: ");
    trig_pin = _trig;
    echo_pin = _echo;
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    //Serial.print("US init . Trig/echo: ");
    //Serial.print(trig_pin);
    //Serial.print("    ");
    //Serial.println(echo_pin);

  } 

  int read_dist() {
    float duration_us, distance_cm;

    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    duration_us  = pulseIn(echo_pin, HIGH,3000 );
    // 3000 is about 50 cm' according to: 50cm = 0.017 * 3000;
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

