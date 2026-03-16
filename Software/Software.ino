/*
  main.ino - Example code for the Pharmacy cross project.
  Created by Vincent Philippoz.

  This code is made for the ESP32-C6 platform and is intended to be used with the custom PCBs.
*/

#include "src\animation\animation.hpp"
#include "src\peripherals\peripherals.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;
constexpr bool DEBUG = true;

void setup() {
    // Initialize serial communication
    if(DEBUG) {
        Serial.begin(SERIAL_BAUD_RATE);
    }

    // Initialize modules
    peripherals::setup(DEBUG);
    animation::setup(DEBUG);
}

void loop() {
    static uint8_t current_animation_id = animation::get_next_animation_id(); // Default animation ID
    static bool change_anim = false;
    static uint8_t brightness = peripherals::get_brightness(); // Default brightness level (0-15)

    // Get user inputs
    change_anim = peripherals::get_next_animation_required();
    uint8_t tmp_brightness = peripherals::get_brightness();

    // Update brightness
    if(tmp_brightness != brightness) {
        brightness = tmp_brightness;
        peripherals::set_brightness(brightness);
    }

    // Update animation if required
    if(change_anim) {
        current_animation_id = animation::get_next_animation_id();
    }

    // Display the next frame of the current animation
    std::vector<std::vector<uint16_t>> frame = animation::get_next_frame();
    peripherals::set_frame(frame);

    // Delay according to the animation speed
    delay(animation::get_animation_speed());
}