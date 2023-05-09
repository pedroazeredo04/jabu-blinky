#ifndef __UTILS_H__
#define __UTILS_H__

#define map(value, former_low, former_high, new_low, new_high) \
    ((value - former_low) * (new_high - new_low) / (former_high - former_low) + new_low)

#endif // __UTILS_H__
