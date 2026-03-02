#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino.h>
#include <vector>

namespace animation {
    // Constants
    constexpr uint8_t TOP = 0;         // Index for the top LED in the frame
    constexpr uint8_t BOTTOM = 1;      // Index for the bottom LED in the frame
    constexpr uint8_t CENTER = 2;      // Index for the center LED in the frame
    constexpr uint8_t ANIM_ON = 0;          // Animation ID for the ON (static) animation
    constexpr uint8_t ANIM_BLINK = 1;       // Animation ID for the blink animation
    constexpr uint8_t ANIM_RANDOM = 2;      // Animation ID for the random animation

    // Function prototypes
    void setup(bool verbose);
    uint8_t select_animation(uint8_t animation_id); // return speed of animation
    uint8_t get_animation_id(void); // return the id of the animation
    uint16_t get_animation_speed(void); // return the speed of the animation
    std::vector<std::vector<uint16_t>> get_next_frame(void); // return pointer to the next frame to display (array of 3*8 uint16_t)

} // namespace animation

#endif // ANIMATION_H