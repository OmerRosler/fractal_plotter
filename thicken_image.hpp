#pragma once
#include "bitmap_image.hpp"

#include <vector>
#include <cstdint>

enum class ColorClass : uint8_t {
    White = 0,
    Black = 1,
    Red = 2,
    Blue = 3,
    Green = 4,
    Yellow = 5
};

ColorClass classify(const rgb_t& c);

rgb_t representative(ColorClass c);

bitmap_image thicken(
    const bitmap_image& input,
    int radius);