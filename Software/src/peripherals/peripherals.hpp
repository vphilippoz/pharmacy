#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <Arduino.h>

namespace peripherals {
    // Constants
    constexpr unsigned int PIN_NEXT_BTN = 1;       // GPIO pin for the "Next" button
    constexpr unsigned int PIN_BRIGHTNESS_POT = 2; // GPIO pin for the "Brightness" potentiometer
    constexpr unsigned int MATRIX_TOP_ADDR = 0x70;    // I2C address for the top LED matrix
    constexpr unsigned int MATRIX_BOTTOM_ADDR = 0x71; // I2C address for
    constexpr unsigned int MATRIX_CENTER_ADDR = 0x72; // I2C address for the center LED matrix

    // Global variables declaration
    // extern bool pumps_active[];

    // Function prototypes
    void setup(bool verbose);
    bool get_next_animation_required();
    uint16_t get_brightness();
    void set_frame(const std::vector<std::vector<uint16_t>>& frame);
    void print_frame(const std::vector<std::vector<uint16_t>>& frame);

} // namespace peripherals

#endif // PERIPHERALS_H