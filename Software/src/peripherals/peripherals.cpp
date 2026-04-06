#include "peripherals.hpp"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"


namespace peripherals {
    
// Global variables definition
bool VERBOSE = false;
bool btn_state = false;
unsigned long last_btn_change_time = 0; // To implement debounce for the button
bool btn_hold = false; // To keep track of whether the button is currently held
bool last_press_was_long = false; // To keep track of whether the last button press was a long press
bool anim_change_required = false; // To indicate that an animation change is required on the next loop iteration
uint8_t current_brightness = 512/POT2BRIGHTNESS_SCALE; // To keep track of the current brightness level
uint16_t current_speed = 512/POT2SPEED_SCALE; // To keep track of the current animation speed
Adafruit_8x16matrix matrix_top = Adafruit_8x16matrix();
Adafruit_8x16matrix matrix_bottom = Adafruit_8x16matrix();
Adafruit_8x16matrix matrix_center = Adafruit_8x16matrix();

// Private function declaration
void update_button_state();

void setup(bool verbose) {
    /**
     * @brief Setup the module
    */
    // Save verbose mode
    VERBOSE = verbose;

    // Initialize GPIOs
    pinMode(PIN_NEXT_BTN, INPUT);
    pinMode(PIN_SPEED_POT, INPUT);

    // // Enable interrupt for the button (active HIGH)
    // attachInterrupt(digitalPinToInterrupt(PIN_NEXT_BTN), peripherals::btn_press_ISR, RISING);

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
    update_button_state();

    if (anim_change_required) {
        anim_change_required = false; // Reset the flag
        return true; // Indicate that an animation change is required
    } else {
        return false; // No animation change required
    }
}

uint8_t get_brightness() {
    /**
     * @brief Get the current brightness level. Using the pot value only if the button is pressed.
     * @return The current brightness level (0-15)
    */
    update_button_state();

    if (btn_state == true && btn_hold == true) {
        current_brightness = analogRead(PIN_SPEED_POT) / POT2BRIGHTNESS_SCALE; 
    }
    return current_brightness;
}

uint16_t get_speed() {
    /**
     * @brief Get the current animation speed. Using the pot value only if the button is not pressed.
     * @return The current animation speed in milliseconds
    */
    update_button_state();

    if (btn_state == false) {
        current_speed = SPEED_OFFSET - (analogRead(PIN_SPEED_POT) / POT2SPEED_SCALE); 
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

void update_button_state() {
    /**
     * @brief Update the button state
    */
    unsigned long now = millis();
    bool measured_btn_state = digitalRead(PIN_NEXT_BTN) == HIGH;

    // Check press/release with debounce
    if (measured_btn_state != btn_state && now - last_btn_change_time > DEBOUNCE_TIME) {
        last_btn_change_time = now;
        btn_state = measured_btn_state;
        if (btn_state == true) {
            // Reset the long press tracking variable when the button is pressed again
            last_press_was_long = false;
        } else {
            // Set the long press tracking variable when the button is released if necessary
            if (btn_hold == true) {
                last_press_was_long = true;
                btn_hold = false;
            } else {
                anim_change_required = true; 
            }
        }
    }

    // Check for long press
    if (btn_state == true && now - last_btn_change_time > LONG_PRESS_TRESHOLD) {
        btn_hold = true;
    }
}


} // namespace peripherals