#include "thicken_image.hpp"
ColorClass classify(const rgb_t& c)
{
    if (c.red == 255 && c.green == 255 && c.blue == 255)
        return ColorClass::White;

    if (c.red == 0 && c.green == 0 && c.blue == 0)
        return ColorClass::Black;

    if (c.red == 255 && c.green == 0 && c.blue == 0)
        return ColorClass::Red;

    if (c.red == 0 && c.green == 0 && c.blue == 255)
        return ColorClass::Blue;

    if (c.red == 0 && c.green == 255 && c.blue == 0)
        return ColorClass::Green;

    if (c.red == 255 && c.green == 255 && c.blue == 0)
        return ColorClass::Yellow;

    return ColorClass::White;
}

rgb_t representative(ColorClass c)
{
    switch (c)
    {
    case ColorClass::Black:
        return make_colour(0, 0, 0);

    case ColorClass::Red:
        return make_colour(255, 0, 0);

    case ColorClass::Blue:
        return make_colour(0, 0, 255);

    case ColorClass::Green:
        return make_colour(0, 255, 0);

    case ColorClass::Yellow:
        return make_colour(255, 255, 0);

    default:
        return make_colour(255, 255, 255);
    }
}

bitmap_image thicken(
    const bitmap_image& input,
    int radius)
{
    const int width = input.width();
    const int height = input.height();

    bitmap_image output(width, height);

    output.set_all_channels(255);

    std::vector<uint8_t> priority(
        width * height,
        0);

    const int r2 = radius * radius;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            rgb_t c;

            input.get_pixel(x, y, c);

            ColorClass cls = classify(c);

            if (cls == ColorClass::White)
                continue;

            uint8_t p = static_cast<uint8_t>(cls);

            for (int dy = -radius; dy <= radius; ++dy)
            {
                for (int dx = -radius; dx <= radius; ++dx)
                {
                    if (dx * dx + dy * dy > r2)
                        continue;

                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx < 0 || ny < 0 ||
                        nx >= width || ny >= height)
                        continue;

                    int idx = ny * width + nx;

                    if (p > priority[idx])
                    {
                        priority[idx] = p;

                        rgb_t out = representative(cls);

                        output.set_pixel(nx, ny, out);
                    }
                }
            }
        }
    }

    return output;
}