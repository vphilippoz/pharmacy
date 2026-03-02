/*
Minimal version to test the module.

This code is a minimal version to test the module. It toggles the defined GPIOs and the corresponding LED color.
*/

#include "animation.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;
constexpr bool VERBOSE = true;
uint8_t current_animation_id = 0; // ID of the current animation
uint8_t frame_counter = 0; 


void setup() {
    if(VERBOSE) {
        // Open serial communications on the native USB port
        Serial.begin(SERIAL_BAUD_RATE);while(!Serial);
    }

    // Setup the library
    animation::setup(VERBOSE);
    animation::select_animation(current_animation_id);
    if(VERBOSE) {Serial.println("Setup complete");}
}

void loop() {
    // Get and print the next frame of the animation
    std::vector<std::vector<uint16_t>> frame = animation::get_next_frame();
    if(VERBOSE) {
        Serial.print("Current animation ID: ");
        Serial.println(animation::get_animation_id());
        Serial.print("Current animation speed: ");
        Serial.println(animation::get_animation_speed());
        for (size_t i = 0; i < frame.size(); i++) {
            Serial.print("Panel ");
            Serial.print(i);
            Serial.print(": ");
            for (size_t j = 0; j < frame[i].size(); j++) {
                Serial.print(frame[i][j], HEX);
                Serial.print(" ");
            }
            Serial.println();
        }
    }
    ++frame_counter;
    
    // After a few frames, switch to the next animation
    if(frame_counter >= 5) {
        current_animation_id++; // Increment animation id
        animation::select_animation(current_animation_id);
        frame_counter = 0; // Reset frame counter
    }
    delay(animation::get_animation_speed()*5); // Wait for the animation speed duration
}