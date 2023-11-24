#include <Arduino.h>
#include "../.pio/libdeps/uno/Adafruit NeoPixel/Adafruit_NeoPixel.h"
#ifdef __AVR__
#include <avr/power.h>
#endif

int const NUM_PIXELS = 12;
int8_t const PIN = 13;
Adafruit_NeoPixel strip;
int ledIndexes[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};



void setup() {
  strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
  strip.begin();
  strip.setBrightness(5); //set Brightness to 5
  strip.show(); // init all pixels "off"
}

enum cases { //TODO: rename
    GAME_TIME,
    ERROR,
    IDLE,
    WORK_IN_PROGRESS
};

void gameTime() {
  for (int y = 0; y < NUM_PIXELS; y++) {
    if (ledIndexes[y] == 1) {
      strip.setPixelColor(y, 0x1F51FF); // Blue color for index with value 1
      ledIndexes[y] = 0;
    } else {
      strip.setPixelColor(y, 0xFFFF33); // Yellow color for index with value 0
      ledIndexes[y] = 1;
    }
  }
}

void error() {
  for (int y = 0; y < NUM_PIXELS; y++) {
    if (ledIndexes[y] == 1) {
      strip.setPixelColor(y, 0xFF0000); // Red color for index with value 1
    } else {
      strip.setPixelColor(y, 0x000000); // Black color for index with value 0
    }
  }
 }

 void idle() {
  for (int y = 0; y < NUM_PIXELS; y++) {
    if (ledIndexes[y] == 1) {
      strip.setPixelColor(y, 0x000000); // Black color for index with value 1
    } else {
      strip.setPixelColor(y, 0x111111); // White color for index with value 0
    }
  }
}

void workInProgress() {
  for (int y = 0; y < NUM_PIXELS; y++) {
    if (ledIndexes[y] == 1) {
      strip.setPixelColor(y, 0xFF0000); // Red color for index with value 1
    } else {
      strip.setPixelColor(y, 0x111111); // White color for index with value 0
    }
  }
}

void colorScheme(cases cases) {
    switch(cases) {
        case GAME_TIME:
            gameTime();
            break;
        case ERROR:
            error();
            break;
        case IDLE:
            idle();
            break;
        case WORK_IN_PROGRESS:
            workInProgress();
            break;
    }
}

cases getCase(int i) {
    if (i == 0) {
        return GAME_TIME;
    }
    if (i == 1) {
        return ERROR;
    }
    if (i == 2) {
        return IDLE;
    }
    if (i == 3) {
        return WORK_IN_PROGRESS;
    }
    return ERROR;
}

void loop() {
    colorScheme(getCase(0)); // Pass the LED indexes array to the function
    strip.show(); // Update NeoPixel strip
    delay(250);

}