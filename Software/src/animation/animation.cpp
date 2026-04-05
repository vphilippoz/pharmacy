#include "animation.hpp"

namespace animation {

// Global variables definition
bool VERBOSE = false;
uint8_t current_animation_id = 0; // ID of the current animation
uint8_t current_frame_id = 0; // ID of the current frame in the animation
uint8_t current_animation_speed = 0; // Added delay between frames [ms]

// Animations data
/*
Each annimation is made of 3 pannels (top, bottom, center) of 16x8 LEDs each. 
One animation can have multiple frames, which are displayed one after the other.
Physically, the center one is only 8x8, so the LSB is ignored.

To access a specific LED, use the following syntax:
uint16_t led_line_values = ANIMATIONS[animation_id][frame_id][panel_id][line_id] 
*/ 
const std::vector<std::vector<std::vector<std::vector<uint16_t>>>> ANIMATIONS = { 
    {   // Animation 0: ON
        {   // Frame 0: All LEDs ON
            {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // Top panel
            {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // Bottom panel
            {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // Center panel
        },
    },
    {   // Animation 1: Blink
        {   // Frame 0: Blink off   
            {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}, // Top panel
            {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}, // Bottom panel
            {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}, // Center panel
        },
        {   // Frame 1: Blink on
            {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // Top panel
            {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // Bottom panel
            {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // Center panel
        },
    },
    {   // Animation 2: Random
        {   // Frame 0: Random 1
            {0x2fd8, 0xbb88, 0xc4a2, 0x0030, 0xea36, 0xa594, 0xb14a, 0x2281}, // Top panel
            {0x9c00, 0xc947, 0x946f, 0xf81b, 0xa79d, 0x5597, 0x31db, 0x0bc7}, // Bottom panel
            {0x2736, 0x5e1a, 0x166d, 0x6005, 0xe615, 0xd3fa, 0x835d, 0x78ab}, // Center panel
        },
        {   // Frame 1: Random 2
            {0x70e5, 0x118c, 0xb291, 0x2115, 0xee70, 0x151b, 0x16ea, 0x2c48}, // Top panel
            {0x41ff, 0x7adf, 0xf7d1, 0x5098, 0x1c71, 0x19ee, 0xc5e5, 0xc60e}, // Bottom panel
            {0xd6c3, 0xcc8e, 0xfa93, 0x7dfb, 0xea10, 0x5653, 0xb22c, 0x1074}, // Center panel
        },
        {   // Frame 2: Random 3
            {0x3100, 0x3e89, 0x1a7b, 0x0dbc, 0xbfc4, 0x6e51, 0x7e27, 0x5e44}, // Top panel
            {0xe43b, 0xdad3, 0x086e, 0x6431, 0x092e, 0x25c6, 0x06a1, 0x4a4d}, // Bottom panel
            {0x1d46, 0x8e33, 0xb89a, 0x9f3c, 0xc9bf, 0x1143, 0x9572, 0x83aa}, // Center panel
        },
    },
    {   // Animation 3: Digits
        {   // Frame 0: Digits
            {0x0000, 0x0000, 0x7E40, 0x087F, 0x0E42, 0x0004, 0x0000, 0x0000}, // Top panel
            {0x0000, 0x0000, 0x2424, 0x542A, 0x5432, 0x5C24, 0x0000, 0x0000}, // Bottom panel
            {0x0000, 0x0000, 0x006C, 0x0092, 0x0092, 0x0044, 0x0000, 0x0000}, // Center panel
        },
    },
    {   // Animation 4: Cross shrinking
        {   // Frame 0: 
            {0xFFFF, 0x8101, 0x8101, 0x8101, 0x8101, 0x8101, 0x8101, 0x81FF}, // Top panel
            {0xFF81, 0x8081, 0x8081, 0x8081, 0x8081, 0x8081, 0x8081, 0xFFFF}, // Bottom panel
            {0x0081, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0081}, // Center panel
        },
        {   // Frame 1: 
            {0x0000, 0x7EFE, 0x4202, 0x4202, 0x4202, 0x4202, 0x42FE, 0x4200}, // Top panel
            {0x0042, 0x7F42, 0x4042, 0x4042, 0x4042, 0x4042, 0x7F7E, 0x0000}, // Bottom panel
            {0x0042, 0x00C3, 0x0000, 0x0000, 0x0000, 0x0000, 0x00C3, 0x0042}, // Center panel
        },
        {   // Frame 2: 
            {0x0000, 0x0000, 0x3CFC, 0x2404, 0x2404, 0x24FC, 0x2400, 0x2400}, // Top panel
            {0x0024, 0x0024, 0x3F24, 0x2024, 0x2024, 0x3F3C, 0x0000, 0x0000}, // Bottom panel
            {0x0024, 0x0024, 0x00E7, 0x0000, 0x0000, 0x00E7, 0x0024, 0x0024}, // Center panel
        },
        {   // Frame 3: 
            {0x0000, 0x0000, 0x0000, 0x18F8, 0x18F8, 0x1800, 0x1800, 0x1800}, // Top panel
            {0x0018, 0x0018, 0x0018, 0x1F18, 0x1F18, 0x0000, 0x0000, 0x0000}, // Bottom panel
            {0x0018, 0x0018, 0x0018, 0x00FF, 0x00FF, 0x0018, 0x0018, 0x0018}, // Center panel
        },
    },
};

const std::vector<uint16_t> ANIMATION_SPEEDS = {
    1000,      // Animation 0: ON (static)
    80,    // Animation 1: Blink (500ms between frames)
    50,    // Animation 2: Random (200ms between frames)
    1000,   // Animation 3: Digits (static)
    100,    // Animation 4: Custom (200ms between frames)
};

void setup(bool verbose) {
    /**
     * @brief Setup the module
    */
    // Save verbose mode
    VERBOSE = verbose;

    // Reset animation state
    current_animation_id = 0;
    current_frame_id = 0;
    current_animation_speed = ANIMATION_SPEEDS[current_animation_id];

    if(VERBOSE) {Serial.println("Animation module initialized successfully");}
}

uint16_t get_animation_speed(void) {
    /**
     * @brief Get the speed of the current animation
     * @return The speed of the current animation
    */
    return ANIMATION_SPEEDS[current_animation_id];
}

uint8_t get_animation_frame_count(void) {
    /**
     * @brief Get the number of frames in the current animation
     * @return The number of frames in the current animation
    */
    return ANIMATIONS[current_animation_id].size();
}

void select_next_animation(void) {
    /**
     * @brief Get the id of the next animation
     * @return The id of the next animation
    */
    current_animation_id = (current_animation_id + 1) % ANIMATIONS.size();
}

std::vector<std::vector<uint16_t>> get_next_frame(void) {
    /**
     * @brief Get the next frame to display
     * @return Next frame to display (vector of 3*8 uint16_t)
    */
    current_frame_id = (current_frame_id + 1) % ANIMATIONS[current_animation_id].size();
    return ANIMATIONS[current_animation_id][current_frame_id];
}

} // namespace animation
