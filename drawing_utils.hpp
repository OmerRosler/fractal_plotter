#pragma once

#include "bitmap_image.hpp"
#include "run_algorithm_per_pixel.hpp"

namespace frc
{
/*
*  A linear algorithm to draw a fractal given a decision algorithm for each pixel
*
* The algorithm result is passed to the "painter function" which decides how to color
* a pixel based on the result
*
* Note that each algorithm has different result semantics and the painter function needs
* to know which algorithm it uses
*
* The painter function is NOT part of the fractal algorithm and therefore decoupled from
* the algorithm class
*/
template<fractal_algorithm IsInFractal>
void draw_fractal(
    const std::string& pic_path,
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations,
    pixel_painter_t<typename IsInFractal::value_t> paint_fn)
{
    assert(domain.is_resolution_for_domain(res));
    bitmap_image fractal_jet(res.width, res.height);

    fractal_jet.clear();

    for (auto&& [pixel, r, escape_index] :
        run_algorithm_per_pixel<IsInFractal>(res, domain, max_iterations))
    {
        color_t result_color = paint_fn(max_iterations, r, escape_index);
        fractal_jet.set_pixel(pixel.x, pixel.y, result_color);
    }

    fractal_jet.save_image(pic_path);
}

}