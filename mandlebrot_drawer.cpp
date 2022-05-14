#include "mandlebrot_drawer.hpp"

namespace frc
{

color_t mandlebrot_pixel_color(unsigned int max_iterations,
    const std::complex<double>& escape_value,
    unsigned int escape_index)
{
    // The pixel did NOT escape and is inside the Mandlebrot set
    if (escape_index == max_iterations)
    {
        //black
        return { 0, 0, 0 };
    }

    // Color formula taken from the bitmap library examples, don't know how it works
    auto index = static_cast<unsigned int>
        (1000.0 * std::log2(1.75 + escape_index -
            std::log2(std::log2(std::abs(escape_value)))) / std::log2(max_iterations));

    return jet_colormap[index];

}


}