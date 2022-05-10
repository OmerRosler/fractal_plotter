#pragma once
#include <format>

#include "utils.hpp"
namespace frc
{
/* Helper function to generate the input to a plotting function when trying to
* zoom in around a given point in a given distance
*/
std::tuple<resolution_t, picture_domain_t, std::string>
inline generate_domain_and_resolution(std::complex<double> pt, double radius)
{
	return { resolution_t{ 600, 600 },
		picture_domain_t{.x{pt.real() - radius, pt.real() + radius},
		.y{pt.imag() - radius, pt.imag() + radius}}, 
		std::format("zoom_pt_{}_{}i_radius{}", pt.real(), pt.imag(), radius) };
}

}