#include <Arduino.h>
#include "../.pio/libdeps/uno/Adafruit NeoPixel/Adafruit_NeoPixel.h"
#ifdef __AVR__
#include <avr/power.h>
#endif

int const NUM_PIXELS = 12;
int8_t const PIN = 13;
Adafruit_NeoPixel strip;
int ledIndexes[NUM_PIXELS];

enum cases {
    GAME_TIME,
    ERROR,
    IDLE,
    WORK_IN_PROGRESS
};

void colorScheme(cases mode);

void setup() {
    Serial.begin(9600);
    Serial.println("Start");
    for (int i = 0; i < NUM_PIXELS; i++) {
        ledIndexes[i] = i % 2 == 0 ? 1 : 0;
    }
    strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.setBrightness(5); //set Brightness to 5
    strip.show(); // init all pixels "off"
}

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

void colorScheme(cases mode) {
    switch(mode) {
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
        default:
            idle();
    }
}

cases getCase() { //TODO: add logic
    if (Serial.available() > 0) {
        String receivedData = Serial.readString();  // Read incoming data
        if (receivedData.equals("GAME_TIME")) {
            return GAME_TIME;
        }
        if (receivedData.equals("ERROR")) {
            return ERROR;
        }
        if (receivedData.equals("IDLE")) {
            return IDLE;
        }
        if (receivedData.equals("WORK_IN_PROGRESS")) {
            return WORK_IN_PROGRESS;
        }
        return IDLE;
    }
    return ERROR;
}

void loop() {
    colorScheme(getCase()); // Pass the LED indexes array to the function
    strip.show(); // Update NeoPixel strip
    delay(250);
}