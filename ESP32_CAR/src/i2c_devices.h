#include <Wire.h>
#include "PCF8574.h"


class i2c_devices{
    // new leds on CAR PCB Aug 2024

    private:
    PCF8574 i2c_ext;

    public:
    i2c_devices(const byte addr = 0x38) : i2c_ext(addr) {}
        
    void i2c_init() {
      i2c_scanner();
      i2c_ext.pinMode(P3,OUTPUT); // LED1
      i2c_ext.pinMode(P4,OUTPUT); // LED2
      i2c_ext.pinMode(P5,OUTPUT); // LED3
      i2c_ext.pinMode(P6,OUTPUT); // LED4
      i2c_ext.pinMode(P7,OUTPUT); // LED5

      for (int i=0;i<3;i++) {
          Led1_Off();
          Led2_Off();
          Led3_Off();
          delay(500);
          Led1_On();
          Led2_On();
          Led3_On();
          delay(500);
      } // of for loop

    } // of I2C_init

    

     void Led1_On() {
        i2c_ext.digitalWrite(P3,LOW);
    } // of Led1_On()

     void Led1_Off() {
        i2c_ext.digitalWrite(P3,HIGH);
    } // of Led1_Off()

    void Led2_On() {
        i2c_ext.digitalWrite(P4,LOW);
    } // of Led1_On()

     void Led2_Off() {
        i2c_ext.digitalWrite(P4,HIGH);
    } // of Led1_Off()

    void Led3_On() {
        i2c_ext.digitalWrite(P5,LOW);
    } // of Led1_On()

     void Led3_Off() {
        i2c_ext.digitalWrite(P5,HIGH);
    } // of Led1_Off()





    void i2c_scanner() {
    byte error, address;
    int nDevices;
    Serial.println("Scanning...");
    nDevices = 0;
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address<16) {
            Serial.print("0");
        }
        Serial.println(address,HEX);
        nDevices++;
        }
        else if (error==4) {
        Serial.print("Unknow error at address 0x");
        if (address<16) {
            Serial.print("0");
        }
        Serial.println(address,HEX);
        }    
    }
    if (nDevices == 0) {
        Serial.println("No I2C devices found\n");
    }
    else {
        Serial.println("done\n");
    }
    delay(5000);   
    } // of i2c_scanner()

    
}; // of I2C_devices class