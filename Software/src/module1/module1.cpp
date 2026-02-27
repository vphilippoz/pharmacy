#include "module1.hpp"

namespace backend {

// Global variables definition
// bool pumps_active[] = {false, false, false, false};
// bool VERBOSE = false;

// Private function declaration
// void set_LED();

void setup(bool verbose) {
    /**
     * @brief Setup the module
    */
    // Save verbose mode
    VERBOSE = verbose;


    if(VERBOSE) {Serial.println("Module initialized successfully");}
}


} // namespace backend
