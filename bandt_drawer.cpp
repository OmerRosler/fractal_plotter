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