#ifndef __RGB_H__
#define __RGB_H__

#include <stdint.h>

typedef struct rgb {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} rgb_t;

rgb_t hue_to_rgb(uint16_t hue);

#endif // __RGB_H__
