/*
  Software.ino - Main code for the Pharmacy cross project.
  Created by Vincent Philippoz.

  This code is made for the ESP32-C6 platform and is intended to be used with the custom PCBs.
*/

#include "src\animation\animation.hpp"
#include "src\peripherals\peripherals.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;
constexpr bool DEBUG = false;

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
    static bool change_anim = false;
    static unsigned long last_frame_change_time = 0; 
    static uint16_t speed = peripherals::get_speed();
    static uint8_t brightness = peripherals::get_brightness();

    // Get user inputs
    uint8_t tmp_brightness = peripherals::get_brightness();
    speed = peripherals::get_speed();

    // Update brightness
    if(tmp_brightness != brightness) {
        brightness = tmp_brightness;
        peripherals::set_brightness(brightness);
    }

    // Update animation if required
    if(peripherals::get_next_animation_required()) {
        animation::select_next_animation();
    }
    
    // Display the next frame of the current animation if the required time has passed
    uint16_t now = millis();
    uint16_t time_between_frames = speed / animation::get_animation_frame_count();
    if (now - last_frame_change_time >= time_between_frames) {
        last_frame_change_time = now;
        std::vector<std::vector<uint16_t>> frame = animation::get_next_frame();
        peripherals::set_frame(frame);
    }

    // Run loop at a reasonable speed
    delay(20); // [ms]
}