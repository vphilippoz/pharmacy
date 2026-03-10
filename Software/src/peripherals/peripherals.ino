/*
Minimal version to test the module.

This code is a minimal version to test the module. It toggles the defined GPIOs and the corresponding LED color.
*/

#include "peripherals.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;
constexpr bool VERBOSE = true;


void setup() {
    if(VERBOSE) {
        // Open serial communications on the native USB port
        Serial.begin(SERIAL_BAUD_RATE);while(!Serial);
    }

    // Setup the library
    peripherals::setup(VERBOSE);
    if(VERBOSE) {Serial.println("Setup complete");}
}

void loop() {
    // Declare variables
    static uint8_t brightness = 0;
    static std::vector<std::vector<uint16_t>> frame = {
        {0x081C, 0x1822, 0x0802, 0x080C, 0x0810, 0x0820, 0x1C3E, 0x8080}, // Top panel
        {0x3E04, 0x020C, 0x0414, 0x0C24, 0x023E, 0x2204, 0x1C04, 0x8080}, // Bottom panel
        {0x3E0E, 0x2010, 0x3C20, 0x023C, 0x0222, 0x2222, 0x1C1C, 0x8080}, // Center panel
    };
    
    // Update the LED matrices
    peripherals::set_brightness(brightness);
    peripherals::set_frame(frame);


    // Print infos
    if(VERBOSE) {
        Serial.print("Brightness: ");
        Serial.println(peripherals::get_brightness());
        Serial.print("Next animation required: ");
        Serial.println(peripherals::get_next_animation_required() ? "Yes" : "No");
        // Serial.println("Current frame: ");
        // peripherals::print_frame(frame);
    }
    
    // Update
    brightness = (brightness + 1) % 16; // Increment brightness level (0-15)
    for (size_t i = 0; i < frame.size(); i++) {
        for (size_t j = 0; j < frame[i].size(); j++) {
            frame[i][j] = ~frame[i][j]; // Invert the bits to create a new pattern
        }
    }
    delay(500); // [ms]
}