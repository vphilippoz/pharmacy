#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino.h>
#include <vector>

namespace animation {
    // Constants
    constexpr uint8_t TOP = 0;         // Index for the top LED in the frame
    constexpr uint8_t BOTTOM = 1;      // Index for the bottom LED in the frame
    constexpr uint8_t CENTER = 2;      // Index for the center LED in the frame

    // Function prototypes
    void setup(bool verbose);
    uint16_t get_animation_speed(void);
    uint8_t get_animation_frame_count(void);
    void select_next_animation(void);
    std::vector<std::vector<uint16_t>> get_next_frame(void);

} // namespace animation

#endif // ANIMATION_H