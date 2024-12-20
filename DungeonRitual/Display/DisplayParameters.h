#pragma once

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

// Screen pixels in 1 meter, can be changed by player
#define DISTANCE_TO_PIXELS 60

// Display pixels in 1 meter, constant value
#define PIXELS_IN_METER 10

// Ratio of pixel scaling from assets to screen
#define PIXEL_SCALEUP (DISTANCE_TO_PIXELS/PIXELS_IN_METER)