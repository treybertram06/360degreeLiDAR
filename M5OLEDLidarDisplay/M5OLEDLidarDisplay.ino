#include <Arduino.h>
#include <M5UnitOLED.h>

M5UnitOLED display;
M5Canvas canvas(&display);

static constexpr size_t textlen = 5;
int textpos = 0;
int scrollstep = 2;

// Define the size of the 2D array
const int arrayWidth = 60;
const int arrayHeight = 60;

// 2D array to hold LiDAR data
int lidarData[arrayWidth][arrayHeight];

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting OLED Setup");
  OLEDSetup();
  fillSampleData(); // Fill the 2D array with sample data
  //drawSquare(); // Draw the square after filling the data
}

void loop() {
  // Your loop code, if needed
  drawSquare();
  Serial.println("Drawing square.");
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
  for (int i = 0; i < arrayWidth; ++i) {
    for (int j = 0; j < arrayHeight; ++j) {
      lidarData[i][j] = 0; // Fill with white pixels
    }
  }
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
  Serial.println("Square drawn with LiDAR data.");
}
