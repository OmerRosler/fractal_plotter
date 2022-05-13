#include "bandt_drawer.hpp"

namespace frc
{


color_t M_pixel_color(unsigned int max_iterations,
    const std::complex<double>& r,
    unsigned int escape_index)
{
    if (bandt_algorithm_functor::is_trivially_inside(r))
    {
        //grey
        return{ 100, 100, 100 };
    }
    // An inconclusive pixel
    else if (escape_index == max_iterations + 1)
    {
        fractal_jet.set_pixel(pixel.x, pixel.y, 0, 0, 255);
    }
    // The validation test precision is too high for the machine
    else if (escape_index == max_iterations + 2)
    {
        fractal_jet.set_pixel(pixel.x, pixel.y, 255, 0, 0);
    }

    // Inside M
    else if (escape_index == max_iterations)
    {
        //black
        return{ 0, 0, 0 };
    }
    else //non-trivially outside
    {
        //white
        return{ 255, 255, 255 };
    }
}

}