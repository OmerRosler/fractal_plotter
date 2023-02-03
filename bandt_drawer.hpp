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
	image_metadata_t meta,
	unsigned int max_iterations);
}