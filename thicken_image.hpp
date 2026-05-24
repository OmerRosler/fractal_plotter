#pragma once
#include "bitmap_image.hpp"

#include <vector>
#include <cstdint>

enum class ColorClass : uint8_t {
    White,
    Black,
    Red,
    Blue,
    Purple,
    Green,
    Yellow
};

ColorClass classify(const rgb_t& c);

rgb_t representative(ColorClass c);

bitmap_image thicken(
    const bitmap_image& input,
    int radius);