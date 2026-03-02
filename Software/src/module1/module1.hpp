#ifndef BACKEND_H
#define BACKEND_H

#include <Arduino.h>

namespace backend {
    // Constants
    constexpr unsigned int CST = 1;       // Example constant

    // Global variables declaration
    // extern bool pumps_active[];

    // Function prototypes
    void setup(bool verbose);

} // namespace backend

#endif // BACKEND_H