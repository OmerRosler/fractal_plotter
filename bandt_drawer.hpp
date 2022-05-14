#pragma once
#include "bandt_algorithm.hpp"
#include "drawing_utils.hpp"
namespace frc
{
    
color_t M_pixel_color(unsigned int max_iterations,
	const std::complex<double>& escape_value,
	unsigned int escape_index);

/*
*  Draws the mandlebrot set at a given resolution in a given domain
*/
inline void draw_M(
    const std::string& pic_path,
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations)
{
    draw_fractal< bandt_algorithm_functor>
        (pic_path, res, domain, max_iterations, M_pixel_color);
}



}
