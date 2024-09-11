#include <Arduino.h>
#include <Wire.h>        // Instantiate the Wire library
#include <TFLI2C.h>      // TFLuna-I2C Library v.0.1.1
#include <M5UnitOLED.h>


TFLI2C tflI2C;

int16_t  tfDist;    // distance in centimeters
int16_t  tfAddr = TFL_DEF_ADR;  // Use this default I2C address

void setup(){
    Serial.begin(115200);  // Initalize serial port
    delay(1000);

    bool i2cConnected = Wire.begin(3, 4);

    if (i2cConnected) {
      Serial.println("I2C Connected");
    } else {
      Serial.println("Error connecting I2C");
    }

}

void loop(){
    if(tflI2C.getData(tfDist, tfAddr)){
        Serial.println(String(tfDist)+" cm / " + String(tfDist/2.54)+" inches");
    }
    delay(10);
}



















