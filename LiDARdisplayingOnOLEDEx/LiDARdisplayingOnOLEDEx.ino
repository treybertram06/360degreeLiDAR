#include <Arduino.h>
#include <Wire.h>        // Instantiate the Wire library
#include <TFLI2C.h>      // TFLuna-I2C Library v.0.1.1
#include <M5UnitOLED.h>

#define ADJUSTMENT_MULTIPLIER 0.1

M5UnitOLED display;
M5Canvas canvas(&display);

TFLI2C tflI2C;

int speedAdjustPin = A0;
double speedAdjust = 0.0;
int speedAdjustInt = 0;

// Define the size of the 2D array
const int arrayWidth = 60;
const int arrayHeight = 60;
int rotations = 0;

// 2D array to hold LiDAR data
int lidarData[arrayWidth][arrayHeight];

int16_t  tfDist;    // distance in centimeters
int16_t  tfAddr = TFL_DEF_ADR;  // Use this default I2C address

void setup(){
  Serial.begin(115200);  // Initalize serial port
  delay(1000);

  bool i2cConnected = Wire.begin(SDA, SCL);

  if (i2cConnected && tflI2C.getData(tfDist, tfAddr)) {
    Serial.println("I2C Connected and data recieved");
  } else {
    Serial.println("Error connecting I2C");
  }

  Serial.println("Starting OLED Setup");
  OLEDSetup();
  Serial.println("Filling sample data.");
  fillSampleData(); // Fill the 2D array with sample data
  //drawSquare(); // Draw the square after filling the data



}

void loop(){

  Serial.println("Looping...");
  speedAdjust = analogRead(speedAdjustPin);
  speedAdjust = speedAdjust * ADJUSTMENT_MULTIPLIER;
  speedAdjustInt = (int)speedAdjust;
  Serial.println(speedAdjust);

  Serial.println(String(tfDist)+" cm");
  
  for (int i=0; i<40; i++) {
    //Serial.println("Start");
    //Serial.println(millis());


    if(tflI2C.getData(tfDist, tfAddr)){
      
      
      if (tfDist < 8000 && tfDist > 5) {
        int x = (tfDist * cos(i*3.14159/180)) / 20;
        int y = (tfDist * sin(i*3.14159/180)) / 20;

        int xIndex = round(30+x);
        int yIndex = round(30-y);
        

        if (xIndex < 60 && xIndex >= 0 && yIndex < 60 && yIndex >= 0) {
          lidarData[xIndex][yIndex] = 1;
        }
      }
      
    } 

    //Serial.println(millis());
    //Serial.println("End");



  }

  
  rotations++;
  if (rotations == 10) {
    fillSampleData();
    rotations = 0;
  }
  
  drawSquare();
  //Serial.println("Drawing square.");
  delay(50);
}

void OLEDSetup() {
  display.init();
  display.setRotation(2);
  canvas.setColorDepth(1); // mono color
  canvas.setFont(&fonts::lgfxJapanMinchoP_32);
  canvas.setTextWrap(false);
  canvas.setTextSize(2);
  canvas.createSprite(display.width() + 64, 72);

  // Verify initialization
  Serial.println("Display and Canvas initialized.");
}

void fillSampleData() {
  for (int i = 0; i < arrayWidth; i++) {
    for (int j = 0; j < arrayHeight; j++) {
      lidarData[i][j] = 0; // Fill with white pixels
    }
  }
  Serial.println("Screen cleared.");
}

void drawSquare() {
  int squareSize = 60; // Size of the square
  int x = 0; // X position
  int y = 0; // Y position

  // Clear the canvas
  canvas.fillScreen(0);

  // Draw hollow square border
  canvas.drawRect(x, y, squareSize, squareSize, 1); // Border thickness is 1px

  // Draw pixels inside the square based on lidarData
  for (int i = 0; i < arrayWidth; ++i) {
    for (int j = 0; j < arrayHeight; ++j) {
      if (lidarData[i][j] == 1) {
        canvas.drawPixel(x + i, y + j, 1); // Draw white pixel
      }
    }
  }

  // Update the display
  canvas.pushSprite(&display, 0, (display.height() - canvas.height()) >> 1);
  display.waitDisplay();
  
  // Add debug information
  //Serial.println("Square drawn with LiDAR data.");
}



















