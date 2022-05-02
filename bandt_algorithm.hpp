#pragma once
#include <complex>
#include <vector>
#include <ranges>

#include "utils.hpp"
#include "bitmap_image.hpp"

inline std::complex<double> g0(std::complex<double> r, std::complex<double> t)
{
	return t / r;
}

inline std::complex<double> g1(std::complex<double> r, std::complex<double> t)
{
	return (t + 1.0) / r;
}

inline std::complex<double> gm1(std::complex<double> r, std::complex<double> t)
{
	return (t - 1.0) / r;
}

std::vector<std::complex<double>> generate_next_set(std::complex<double> r,
	const std::vector<std::complex<double>>& previous_set);

unsigned int bandt_algorithm(std::complex<double> r, unsigned int max_iterations);

/* Yields:
*  - the starting pixel
*  - the magnitude of the initial value
*  - the index of escape
*/
std::generator<std::tuple<pixel_coordinates, double, unsigned int>>
M_pixel_map(
	resolution_t res,
	picture_domain_t domain,
	unsigned int max_iterations);

/*
*  Draws the mandlebrot set at a given resolution in a given domain
*/
void draw_M(
	const std::string& pic_path,
	resolution_t res,
	picture_domain_t domain,
	unsigned int max_iterations);