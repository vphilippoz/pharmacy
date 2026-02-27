/*
  main.ino - Example code for the Pharmacy cross project.
  Created by Vincent Philippoz.

  This code is made for the ESP32-C6 platform and is intended to be used with the custom PCBs.
*/

#include "src\module1\module1.hpp"
#include "src\module2\module2.hpp"

// Constants
constexpr unsigned int SERIAL_BAUD_RATE = 115200;
constexpr bool DEBUG = true;

void setup() {
    // Initialize serial communication
    if(DEBUG) {
        Serial.begin(SERIAL_BAUD_RATE);
    }

    // Initialize modules
}

void loop() {
    // Main loop
    pass;
}