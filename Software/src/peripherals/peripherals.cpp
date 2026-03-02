#include "peripherals.hpp"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"


namespace peripherals {
    
// Global variables definition
bool VERBOSE = false;
volatile bool next_animation_required = false;
Adafruit_8x16matrix matrix_top = Adafruit_8x16matrix();
Adafruit_8x16matrix matrix_bottom = Adafruit_8x16matrix();
Adafruit_8x16matrix matrix_center = Adafruit_8x16matrix();

// Private function declaration
void IRAM_ATTR next_btn_ISR();

void setup(bool verbose) {
    /**
     * @brief Setup the module
    */
    // Save verbose mode
    VERBOSE = verbose;

    // Initialize GPIOs
    pinMode(PIN_NEXT_BTN, INPUT);
    pinMode(PIN_BRIGHTNESS_POT, INPUT);

    // Enable interrupt for the "Next" button (assuming it is active LOW)
    attachInterrupt(digitalPinToInterrupt(PIN_NEXT_BTN), peripherals::next_btn_ISR, FALLING);
    
    // Initialize LED matrices
    matrix_top.begin(MATRIX_TOP_ADDR);
    matrix_bottom.begin(MATRIX_BOTTOM_ADDR);
    matrix_center.begin(MATRIX_CENTER_ADDR);
    matrix_top.setDisplayState(true);
    matrix_bottom.setDisplayState(true);
    matrix_center.setDisplayState(true);

    if(VERBOSE) {Serial.println("Module initialized successfully");}
}

bool get_next_animation_required() {
    /**
     * @brief Check if the next animation frame is required
     * @return true if the next animation frame is required, false otherwise
    */
    noInterrupts(); // Disable interrupts to ensure atomic access to the volatile variable
    bool required = next_animation_required;
    next_animation_required = false; // Reset the flag
    interrupts(); // Re-enable interrupts
    return required;
}

uint8_t get_brightness() {
    /**
     * @brief Get the current brightness level
     * @return The current brightness level (0-15)
    */
    return analogRead(PIN_BRIGHTNESS_POT) / POT2BRIGHTNESS_SCALE; 
}

void set_brightness(uint8_t brightness) {
    /**
     * @brief Set the brightness level for the LED matrices
     * @param brightness The brightness level to be set (0-15)
    */
    // Ensure brightness is within the valid range
    brightness = brightness > 15 ? 15 : brightness;
    
    // Set brightness for all matrices
    matrix_top.setBrightness(brightness);
    matrix_bottom.setBrightness(brightness);
    matrix_center.setBrightness(brightness);
}

void set_frame(const std::vector<std::vector<uint16_t>>& frame) {
    /**
     * @brief Set the current frame to be displayed
     * @param frame The frame to be displayed, represented as a 3x8 vector of uint16_t values
    */
    for (uint8_t i=0; i<8; i++) {
        matrix_top.displaybuffer[i] = frame[0][i];
        matrix_bottom.displaybuffer[i] = frame[1][i];
        matrix_center.displaybuffer[i] = frame[2][i];
    }
    matrix_top.writeDisplay();
    matrix_bottom.writeDisplay();
    matrix_center.writeDisplay();
}

void print_frame(const std::vector<std::vector<uint16_t>>& frame) {
    /**
     * @brief Print the current frame to the serial monitor
     * @param frame The frame to be printed, represented as a 3x8 vector of uint16_t values
    */
    if(!VERBOSE) return; // Do nothing if not in verbose mode

    // Print top panel
    for (uint8_t i = 0; i < 8; i++) { // 8 lines per panel
        // Padding
        Serial.print("                ");
        for (uint8_t j = 0; j < 8; j++) { 
            if (frame[0][i] & 1<<(15-j)) { // MSB
                Serial.print("O ");
            } else {
                Serial.print(". ");
            }
        }
        Serial.println("                ");
    }
    // Print middle panels
    for (uint8_t i = 0; i < 8; i++) { // 8 lines per panel
        // Left panel
        for (uint8_t j = 0; j < 8; j++) {
            if (frame[0][i] & 1<<(7-j)) { // LSB
                Serial.print("O ");
            } else {
                Serial.print(". ");
            }
        }
        // Centre panel
        for (uint8_t j = 0; j < 8; j++) {
            if (frame[2][i] & 1<<(15-j)) { // MSB
                Serial.print("O ");
            } else {
                Serial.print(". ");
            }
        }
        // Right panel
        for (uint8_t j = 0; j < 8; j++) {
            if (frame[1][i] & 1<<(15-j)) { // MSB
                Serial.print("O ");
            } else {
                Serial.print(". ");
            }
        }
        Serial.println();
    }
    // Print bottom panel
    for (uint8_t i = 0; i < 8; i++) { // 8 lines per panel
        // Padding
        Serial.print("                ");
        for (uint8_t j = 0; j < 8; j++) { 
            if (frame[1][i] & 1<<(7-j)) { // LSB
                Serial.print("O ");
            } else {
                Serial.print(". ");
            }
        }
        Serial.println("                ");
    }

}

void IRAM_ATTR next_btn_ISR() {
    /**
     * @brief Interrupt Service Routine for the "Next" button
    */
    next_animation_required = true;
}

} // namespace peripherals


/* Example of a frame
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
O O O O O O O O O O O O O O O O O O O O O O O O 
O O O O O O O O O O O O O O O O O O O O O O O O 
O O O O O O O O O O O O O O O O O O O O O O O O 
O O O O O O O O O O O O O O O O O O O O O O O O 
O O O O O O O O O O O O O O O O O O O O O O O O 
O O O O O O O O O O O O O O O O O O O O O O O O 
O O O O O O O O O O O O O O O O O O O O O O O O 
O O O O O O O O O O O O O O O O O O O O O O O O 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
. . . . . . . . O O O O O O O O . . . . . . . . 
*/