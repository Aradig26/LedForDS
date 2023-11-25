#include <Arduino.h>
#include "../.pio/libdeps/uno/Adafruit NeoPixel/Adafruit_NeoPixel.h"
#ifdef __AVR__
#include <avr/power.h>
#endif

int const NUM_PIXELS = 100; // Number of pixels in the NeoPixel strip TODO: Change
int8_t const PIN = 13; // Pin number for the NeoPixel strip TODO: Change
Adafruit_NeoPixel strip;
int ledIndexes[NUM_PIXELS]; // Array to store the indexes of the LEDs

//For blinking colors method
unsigned long previousMillis = 0;
bool colorToggle = false; // Variable to toggle between colors


enum COLORS_467 { //basic COLORS we use for the LEDs
    Black = 0x000000,
    White = 0xFFFFFF,
    Red = 0xFF0000,
    Green = 0x00FF00,
    Blue = 0x0000FF,
    Yellow = 0xFFFF00,
    Purple = 0x800080,
    Orange = 0xFFA500
};

const COLORS_467 ARM_UNCALIBRATED_COLOR = Red;
const COLORS_467 BATTERY_LOW_COLOR = Orange;

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
    strip.setBrightness(255); //set Brightness of the LEDs
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
void set(COLORS_467 color) {
    for (int y = 0; y < NUM_PIXELS; y++) {
        strip.setPixelColor(y, color);
    }
}
void setTop(COLORS_467 color) {
    for (int y = 0; y < NUM_PIXELS / 2; y++) {
        strip.setPixelColor(y, color);
    }
}
void setBottom(COLORS_467 color) {
    for (int y = NUM_PIXELS / 2; y < NUM_PIXELS; y++) {
        strip.setPixelColor(y, color);
    }
}
void setOneThird(COLORS_467 color, int third) {
    int setMinPixels = round(third == 1 ? 0 : third == 2 ? NUM_PIXELS/3 : (NUM_PIXELS/3)*2);
    int setMaxPixels = round(third == 1 ? NUM_PIXELS/3 : third == 2 ? (NUM_PIXELS/3)*2 : NUM_PIXELS);
    for (int y = 0; y < NUM_PIXELS; y++) {
        if (y>=setMinPixels && y<setMaxPixels) {
            strip.setPixelColor(y, color);
        } else {
            strip.setPixelColor(y, Black);
        }
    }
}
void setBlinkColors(COLORS_467 color1, COLORS_467 color2) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;

        if (colorToggle) {
            for (int y = 0; y < NUM_PIXELS; y++) {
                strip.setPixelColor(y, color1);
            }
        } else {
            for (int y = 0; y < NUM_PIXELS; y++) {
                strip.setPixelColor(y, color2);
            }
        }
        colorToggle = !colorToggle;
    }
}
void setColorMovingUp(COLORS_467 fgColor, COLORS_467 bgColor) {
    static int i = 0;
    for (int y = 0; y < NUM_PIXELS; y++) {
        strip.setPixelColor(y, bgColor);
    }
    if (i <= NUM_PIXELS) {
        delay(100);
        strip.setPixelColor(i, fgColor);
        i++;
    } else {
        i = 0;
    }
}
void setColorMovingDown(COLORS_467 fgColor, COLORS_467 bgColor) {
    static int i = NUM_PIXELS;
    for (int y = 0; y < NUM_PIXELS; y++) {
        strip.setPixelColor(y, bgColor);
    }
    if (i <= NUM_PIXELS&&i>=0) {
        delay(100);
        strip.setPixelColor(i, fgColor);
        i--;
    } else {
        i = NUM_PIXELS;
    }
}
void balanceVictoryLeds() {
    for (int y = 0; y < NUM_PIXELS; y++) {
        if (y % 2 == 0) {
            strip.setPixelColor(y, Blue); // Blue color for index with value 1
        } else {
            strip.setPixelColor(y, Yellow); // Yellow color for index with value 0
        }
    }
}
void scoreVictoryLeds() {
    for (int y = 0; y < NUM_PIXELS; y++) {
        if (y % 2 == 0) {
            strip.setPixelColor(y, Blue); // Blue color for index with value 1
        } else {
            strip.setPixelColor(y, Yellow); // Yellow color for index with value 0
        }
    }
}
void rainbowLed() {
    static uint16_t startIndex = 0;
    static uint16_t offset = 0;

    startIndex = (startIndex + 2) % 360;
    offset = (offset + 2) % strip.numPixels(); // Offset to shift LED positions

    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        int pixelHue = startIndex + (i *2.2);
        pixelHue = pixelHue % 360; // Keep hue value within 0-359

        int pixelPos = (i + offset) % strip.numPixels(); // Calculate new pixel position

        strip.setPixelColor(pixelPos, strip.gamma32(strip.ColorHSV(pixelHue * 65536L / 360))); // Set pixel color
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
            setOneThird(Yellow, 3);
            break;
        case CONE_LOW:
            setOneThird(Yellow, 1);
            break;
        case CONE_MID:
            setOneThird(Yellow, 2);
            break;
        case CUBE_HIGH:
            setOneThird(Purple, 3);
            break;
        case CUBE_LOW:
            setOneThird(Purple, 1);
            break;
        case CUBE_MID:
            setOneThird(Purple, 2);
            break;
        case HOLD_CONE:
            setBlinkColors(Yellow, White);
            break;
        case HOLD_CUBE:
            setBlinkColors(Purple, White);
            break;
        case INTAKE_CONE:
            setColorMovingUp(White, Yellow);
            break;
        case INTAKE_CUBE:
            setColorMovingUp(White, Purple);
            break;
        case RELEASE_CONE:
            setColorMovingDown(Black, Yellow);
            break;
        case RELEASE_CUBE:
            setColorMovingDown(Black, Purple);
            break;
        case WANT_CONE:
            set(Yellow);
            break;
        case WANT_CUBE:
            set(Purple);
            break;
        case INTAKE_UNKNOWN:
            setColorMovingUp(Purple, Yellow);
            break;
        case RELEASE_UNKNOWN:
            setColorMovingDown(Yellow, Purple);
            break;
        case CALIBRATING:
            setBlinkColors(ARM_UNCALIBRATED_COLOR, Black);
            break;
        case SHELF:
            setTop(Purple);
            setBottom(Yellow);
            break;
        case FLOOR:
            setBottom(Purple);
            setTop(Yellow);
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