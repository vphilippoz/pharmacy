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
Physically, the center one is only 8x8, so the MSB is ignored.

To access a specific LED, use the following syntax:
uint16_t led_line_values = ANIMATIONS[animation_id][frame_id][panel_id][line_id] 
*/ 
const std::vector<std::vector<std::vector<std::vector<uint16_t>>>> ANIMATIONS = { 
    {   // Animation 0: ON
        {   // Frame 0: All LEDs ON
            {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // Top panel
            {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}, // Bottom panel
            {0x00FF, 0x00FF, 0x00FF, 0x00FF, 0x00FF, 0x00FF, 0x00FF, 0x00FF}, // Center panel
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
            {0x00FF, 0x00FF, 0x00FF, 0x00FF, 0x00FF, 0x00FF, 0x00FF, 0x00FF}, // Center panel
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
            {0x081C, 0x1822, 0x0802, 0x080C, 0x0810, 0x0820, 0x1C3E, 0x8080}, // Top panel
            {0x3E04, 0x020C, 0x0414, 0x0C24, 0x023E, 0x2204, 0x1C04, 0x8080}, // Bottom panel
            {0x3E0E, 0x2010, 0x3C20, 0x023C, 0x0222, 0x2222, 0x1C1C, 0x8080}, // Center panel
        },
    },
};

const std::vector<uint16_t> ANIMATION_SPEEDS = {
    1000,      // Animation 0: ON (static)
    500,    // Animation 1: Blink (500ms between frames)
    200,    // Animation 2: Random (200ms between frames)
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

    if(VERBOSE) {Serial.println("Module initialized successfully");}
}

void select_animation(uint8_t animation_id) {
    /**
     * @brief Select the animation to display
     * @param animation_id The id of the animation to display
    */
    current_animation_id = animation_id % ANIMATIONS.size();
    current_frame_id = 0; // Reset frame id to 0 when selecting a new animation
}

uint16_t get_animation_speed(void) {
    /**
     * @brief Get the speed of the current animation
     * @return The speed of the current animation
    */
    return ANIMATION_SPEEDS[current_animation_id];
}

uint8_t get_next_animation_id(void) {
    /**
     * @brief Get the id of the next animation
     * @return The id of the next animation
    */
    current_animation_id = (current_animation_id + 1) % ANIMATIONS.size();
    return current_animation_id;
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
