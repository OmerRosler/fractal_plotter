#pragma once

#include "N_algorithm.hpp"
#include "drawing_utils.hpp"
namespace frc
{

color_t N_pixel_color(unsigned int max_iterations,
	const r2vec_t& escape_value,
	unsigned int escape_index);

/*
*  Draws the mandlebrot set at a given resolution in a given domain
*/
inline void draw_N(
    const std::string& pic_path,
	image_metadata_t meta,
    unsigned int max_iterations)
{
    draw_fractal< N_algorithm_functor>
        (pic_path, meta, max_iterations, N_pixel_color);

}

}