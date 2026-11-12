#ifndef COMMON_HPP
#define COMMON_HPP
#include <cstdint>

namespace processing {
    struct MotionLocation {
        uint32_t x;
        uint32_t y;
        bool fire;

        MotionLocation(uint32_t x, uint32_t y, bool fire);
    };
}

#endif
