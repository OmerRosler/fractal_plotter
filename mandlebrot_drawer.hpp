#pragma once
#include <complex>

#include "generator.hpp"
#include "bitmap_image.hpp"
#include "utils.hpp"

/* Takes a complex number c, iterates z^2+c until |z|^2>4 or max_iterations
* It sets `c` the last iterate and returns the index in which the cutoff was passed
* 
* If we reach `max_iterations` without the cut-off it returns `max_iterations + 1`
*/
unsigned int iterate_mandlebrot(std::complex<double>& c, unsigned int max_iterations);

/* Yields:
*  - the starting pixel
*  - the magnitude of the iterate when it exceeds 4
*  - the index of escape
*/
std::generator<std::tuple<pixel_coordinates, double, unsigned int>>
mandlebrot_pixel_map(
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations);

/*
*  Draws the mandlebrot set at a given resolution in a given domain
*/
void draw_mandlebrot(
    const std::string& pic_path, 
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations);