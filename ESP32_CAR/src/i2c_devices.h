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
      i2c_ext.pinMode(P2,OUTPUT); // Buzzer
    } // of I2C_init

    void led_green_on() {Led_On(I2C_led1);}
    void led_green_off() {Led_Off(I2C_led1);}
    void led_yellow_on() {Led_On(I2C_led2);}
    void led_yellow_off() {Led_Off(I2C_led2);}
    void led_red_on() {Led_On(I2C_led3);}
    void led_red_off() {Led_Off(I2C_led3);}
    void leds_ctrl(bool _green, bool _yellow,bool _red) {
        if (_green==true) led_green_on(); else led_green_off();
        if (_yellow==true) led_yellow_on(); else led_yellow_off();
        if (_red==true) led_red_on(); else led_red_off();
    } // of leds_ctrl
    


    void test_I2C_devs(){
        Serial.println("Checking I2C devices (leds and buzzer)");
        for (int i=0;i<6;i++) {
            Buz_On();
            Led_On(I2C_led1);
            Led_On(I2C_led2);
            Led_On(I2C_led3);
            Led_On(I2C_led4);
            Led_On(I2C_led5);
            delay(500);
            Buz_Off();
            Led_Off(I2C_led1);
            Led_Off(I2C_led2);
            Led_Off(I2C_led3);
            Led_Off(I2C_led4);
            Led_Off(I2C_led5);
            delay(500);
        } // of for loop
    } // of I2C_devices_check


     void Led_On(int _led) {
        i2c_ext.digitalWrite(_led,LOW);
    } // of Led1_On()

     void Led_Off(int _led) {
        i2c_ext.digitalWrite(_led,HIGH);
    } // of Led1_On()


    void Buz_On() {
        i2c_ext.digitalWrite(I2C_buzzer,HIGH);
    } // of Led1_On()

     void Buz_Off() {
        i2c_ext.digitalWrite(I2C_buzzer,LOW);
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