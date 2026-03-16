#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <Arduino.h>

namespace peripherals {
    // Constants
    constexpr unsigned int PIN_NEXT_BTN = 10;       // GPIO pin for the "Next" button
    constexpr unsigned int PIN_BRIGHTNESS_POT = 0; // GPIO pin for the "Brightness" potentiometer
    constexpr unsigned int PIN_I2C_SDA = 21;        // GPIO pin for I2C Data
    constexpr unsigned int PIN_I2C_SCL = 22;        // GPIO pin for I2C Clock
    constexpr unsigned int MATRIX_TOP_ADDR = 0x71;    // I2C address for the top LED matrix
    constexpr unsigned int MATRIX_BOTTOM_ADDR = 0x70; // I2C address for the bottom LED matrix
    constexpr unsigned int MATRIX_CENTER_ADDR = 0x72; // I2C address for the center LED matrix
    constexpr unsigned int POT2BRIGHTNESS_SCALE = 256; // Scale factor to convert potentiometer value (0-1023) to brightness level (0-15)

    // Function prototypes
    void setup(bool verbose);
    bool get_next_animation_required();
    uint8_t get_brightness();
    void set_brightness(uint8_t brightness);
    void set_frame(const std::vector<std::vector<uint16_t>>& frame);
    // void print_frame(const std::vector<std::vector<uint16_t>>& frame); Not correctly implemented yet, needs to be adapted to the hardware 

} // namespace peripherals

#endif // PERIPHERALS_H