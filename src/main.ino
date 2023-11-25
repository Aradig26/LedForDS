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
    WANT_CUBE,
    WANT_CONE,
    HOLD_CUBE,
    HOLD_CONE,
    INTAKE_CUBE,
    INTAKE_CONE,
    RELEASE_CUBE,
    RELEASE_CONE,
    BATTERY_LOW,
    ARM_UNCALIBRATED,
    CUBE_LOW,
    CUBE_MID,
    CUBE_HIGH,
    CONE_LOW,
    CONE_MID,
    CONE_HIGH,
    INTAKE_UNKNOWN,
    RELEASE_UNKNOWN,
    CALIBRATING,
    FLOOR,
    SHELF,
    BALANCE_VICTORY,
    AUTO_SCORE,
    IDLE,
    ERROR
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

void error() {
    for (int y = 0; y < NUM_PIXELS; y++) {
        if (ledIndexes[y] == 1) {
            strip.setPixelColor(y, 0xFF0000); // Red color for index with value 1
        } else {
            strip.setPixelColor(y, 0x000000); // Black color for index with value 0
        }
    }
}



void colorScheme(cases mode) {
    switch (mode) {
        case BATTERY_LOW:
            set(BATTERY_LOW_COLOR);
            break;
        case ARM_UNCALIBRATED:
            set(ARM_UNCALIBRATED_COLOR);
            break;
        case CONE_HIGH:
            setOneThird(COLORS_467.Yellow, 3);
            break;
        case CONE_LOW:
            setOneThird(COLORS_467.Yellow, 1);
            break;
        case CONE_MID:
            setOneThird(COLORS_467.Yellow, 2);
            break;
        case CUBE_HIGH:
            setOneThird(COLORS_467.Purple, 3);
            break;
        case CUBE_LOW:
            setOneThird(COLORS_467.Purple, 1);
            break;
        case CUBE_MID:
            setOneThird(COLORS_467.Purple, 2);
            break;
        case HOLD_CONE:
            setBlinkColors(
                    COLORS_467.Yellow, COLORS_467.White);
            break;
        case HOLD_CUBE:
            setBlinkColors(
                    COLORS_467.Purple, COLORS_467.White);
            break;
        case INTAKE_CONE:
            setColorMovingUp(COLORS_467.White, COLORS_467.Yellow);
            break;
        case INTAKE_CUBE:
            setColorMovingUp(COLORS_467.White, COLORS_467.Purple);
            break;
        case RELEASE_CONE:
            setColorMovingDown(COLORS_467.Black, COLORS_467.Yellow);
            break;
        case RELEASE_CUBE:
            setColorMovingDown(COLORS_467.Black, COLORS_467.Purple);
            break;
        case WANT_CONE:
            set(COLORS_467.Yellow);
            break;
        case WANT_CUBE:
            set(COLORS_467.Purple);
            break;
        case INTAKE_UNKNOWN:
            setColorMovingUp(
                    COLORS_467.Purple, COLORS_467.Yellow);
            break;
        case RELEASE_UNKNOWN:
            setColorMovingDown(
                    COLORS_467.Yellow, COLORS_467.Purple);
            break;
        case CALIBRATING:
            setBlinkColors(
                    ARM_UNCALIBRATED_COLOR, ARM_UNCALIBRATED_COLOR, COLORS_467.Black);
            break;
        case SHELF:
            setTop(COLORS_467.Purple);
            setBottom(COLORS_467.Yellow);
            break;
        case FLOOR:
            setBottom(COLORS_467.Purple);
            setTop(COLORS_467.Yellow);
            break;
        case BALANCE_VICTORY:
            balanceVictoryLeds();
            break;
        case AUTO_SCORE:
            scoreVictoryLeds();
            break;
        case IDLE:
            rainbowLed();
            break;
        case ERROR:
            error();
            break;
        default:
            rainbowLed();
            break;
    }
}

cases getCase() {
    if (Serial.available() > 0) {
        String receivedData = Serial.readString();  // Read incoming data

        // Check if the battery is low
        if (receivedData.equals("BATTERY_LOW")) {
            return BATTERY_LOW;
        }
        // Check if arm is calibrated
        if (receivedData.equals("ARM_UNCALIBRATED")) {
            return ARM_UNCALIBRATED;
        }
        if (receivedData.equals("BALANCE_VICTORY")) {
            return BALANCE_VICTORY;
        }
        if (receivedData.equals("AUTO_SCORE")) {
            return AUTO_SCORE;
        }

        // When the arm is calibrating
        if (receivedData.equals("CALIBRATING")) {
            return CALIBRATING;
        }

        // When arm is scoring high
        if (receivedData.equals("CUBE_HIGH")) {
            return CUBE_HIGH;
        }
        if (receivedData.equals("CONE_HIGH")) {
            return CONE_HIGH;
        }

        // When arm is scoring mid-node
        if (receivedData.equals("CUBE_MID")) {
            return CUBE_MID;
        }
        if (receivedData.equals("CONE_MID")) {
            return CONE_MID;
        }

        // When arm is scoring hybrid/low node
        if (receivedData.equals("CUBE_LOW")) {
            return CUBE_LOW;
        }
        if (receivedData.equals("CONE_LOW")) {
            return CONE_LOW;
        }

        // When picking up game pieces from shelf
        if (receivedData.equals("SHELF")) {
            return SHELF;
        }

        // When picking up game pieces from the floor
        if (receivedData.equals("FLOOR")) {
            return FLOOR;
        }

        // If trying to hold on to something
        if (receivedData.equals("HOLD_CUBE")) {
            return HOLD_CUBE;
        }
        if (receivedData.equals("HOLD_CONE")) {
            return HOLD_CONE;
        }

        // If trying to intake something
        if (receivedData.equals("INTAKE_CUBE")) {
            return INTAKE_CUBE;
        }
        if (receivedData.equals("INTAKE_CONE")) {
            return INTAKE_CONE;
        }
        if (receivedData.equals("INTAKE_UNKNOWN")) {
            return INTAKE_UNKNOWN;
        }

        // If trying to release something
        if (receivedData.equals("RELEASE_CUBE")) {
            return RELEASE_CUBE;
        }
        if (receivedData.equals("RELEASE_CONE")) {
            return RELEASE_CONE;
        }
        if (receivedData.equals("RELEASE_UNKNOWN")) {
            return RELEASE_UNKNOWN;
        }

        // If we want cubes
        if (receivedData.equals("WANT_CUBE")) {
            return WANT_CUBE;
        }

        // If we want cones
        if (receivedData.equals("WANT_CONE")) {
            return WANT_CONE;
        }

        //If default case
        if (receivedData.equals("DEFAULT")) {
            return IDLE;
        }

        // Sets ERROR if nothing received (never used)
        return ERROR;
    }

    return ERROR;
}

void loop() {
    colorScheme(getCase()); // Pass the LED indexes array to the function
    strip.show(); // Update NeoPixel strip
    delay(250);
}