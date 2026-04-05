#include "peripherals.hpp"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"


namespace peripherals {
    
// Global variables definition
bool VERBOSE = false;
volatile bool next_animation_required = false;
volatile unsigned long last_press_time = 0; // To implement debounce for the button
uint8_t current_brightness = 512/POT2BRIGHTNESS_SCALE; // To keep track of the current brightness level
uint16_t current_speed = 512/POT2SPEED_SCALE; // To keep track of the current animation speed
Adafruit_8x16matrix matrix_top = Adafruit_8x16matrix();
Adafruit_8x16matrix matrix_bottom = Adafruit_8x16matrix();
Adafruit_8x16matrix matrix_center = Adafruit_8x16matrix();

// Private function declaration
void IRAM_ATTR btn_press_ISR();
bool get_button_state();

void setup(bool verbose) {
    /**
     * @brief Setup the module
    */
    // Save verbose mode
    VERBOSE = verbose;

    // Initialize GPIOs
    pinMode(PIN_NEXT_BTN, INPUT);
    pinMode(PIN_SPEED_POT, INPUT);

    // Enable interrupt for the button (active HIGH)
    attachInterrupt(digitalPinToInterrupt(PIN_NEXT_BTN), peripherals::btn_press_ISR, RISING);

    // Initialize LED matrices
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    matrix_top.begin(MATRIX_TOP_ADDR, &Wire);
    matrix_bottom.begin(MATRIX_BOTTOM_ADDR, &Wire);
    matrix_center.begin(MATRIX_CENTER_ADDR, &Wire);
    matrix_top.setDisplayState(true);
    matrix_bottom.setDisplayState(true);
    matrix_center.setDisplayState(true);

    if(VERBOSE) {Serial.println("Peripherals module initialized successfully");}
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
     * @brief Get the current brightness level. Using the pot value only if the button is pressed.
     * @return The current brightness level (0-15)
    */
    if (get_button_state() == true) {
        current_brightness = analogRead(PIN_SPEED_POT) / POT2BRIGHTNESS_SCALE; 
    }
    return current_brightness;
}

uint16_t get_speed() {
    /**
     * @brief Get the current animation speed. Using the pot value only if the button is not pressed.
     * @return The current animation speed in milliseconds
    */
    if (get_button_state() == false) {
        current_speed = analogRead(PIN_SPEED_POT) / POT2SPEED_SCALE; 
    }
    return current_speed;
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

void IRAM_ATTR btn_press_ISR() {
    /**
     * @brief Interrupt Service Routine for the "Next" button
    */
    unsigned long now = millis();
    if (now - last_press_time > DEBOUNCE_TIME) {
        last_press_time = now;
        next_animation_required = true;
    }
}

bool get_button_state() {
    /**
     * @brief Get the current state of the button
     * @return true if the button is currently pressed, false otherwise
    */
    return digitalRead(PIN_NEXT_BTN) == HIGH;
}

} // namespace peripherals