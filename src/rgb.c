#include "rgb.h"
#include "utils.h"

rgb_t hue_to_rgb(uint16_t hue) {
    uint16_t sector = hue / 682; // 682 = 4096 / 6
    uint16_t position_in_sector = hue % 682;

    uint16_t rising = map(position_in_sector, 0, 681, 0, 1000);
    uint16_t falling = map(position_in_sector, 0, 681, 1000, 0);

    rgb_t rgb;

    switch (sector) {
        case 0: // Red -> Yellow
            rgb.r = 1000;
            rgb.g = rising;
            rgb.b = 0;
            break;

        case 1: // Yellow -> Green
            rgb.r = falling;
            rgb.g = 1000;
            rgb.b = 0;
            break;

        case 2: // Green -> Cyan
            rgb.r = 0;
            rgb.g = 1000;
            rgb.b = rising;
            break;

        case 3: // Cyan -> Blue
            rgb.r = 0;
            rgb.g = falling;
            rgb.b = 1000;
            break;

        case 4: // Blue -> Magenta
            rgb.r = rising;
            rgb.g = 0;
            rgb.b = 1000;
            break;

        case 5: // Magenta -> Red
            rgb.r = 1000;
            rgb.g = 0;
            rgb.b = falling;
            break;
    }

    return rgb;
}
