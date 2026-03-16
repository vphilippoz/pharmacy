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
        {0x0000, 0x0000, 0x7E40, 0x087F, 0x0E42, 0x0004, 0x0000, 0x0000}, // Top panel
        {0x0000, 0x0000, 0x2424, 0x542A, 0x5432, 0x5C24, 0x0000, 0x0000}, // Bottom panel
        {0x0000, 0x0000, 0x006C, 0x0092, 0x0092, 0x0044, 0x0000, 0x0000}, // Center panel
    };
    
    // Update the LED matrices
    peripherals::set_brightness(2);
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
    delay(200); // [ms]
}