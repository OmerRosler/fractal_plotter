#pragma once
#include "bandt_algorithm.hpp"
#include "bitmap_image.hpp"
#include "run_algorithm_per_pixel.hpp"
namespace frc
{
/*
*  Draws the mandlebrot set at a given resolution in a given domain
*/
void draw_M(
	const std::string& pic_path,
	resolution_t res,
	picture_domain_t domain,
	unsigned int max_iterations);
}