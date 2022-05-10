#include "N_drawer.hpp"

namespace frc
{
static_assert(r2_fractal_algorithm<N_algorithm_functor>);

void draw_N(
    const std::string& pic_path,
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations)
{
    assert(domain.is_resolution_for_domain(res));
    bitmap_image fractal_jet(res.width, res.height);

    fractal_jet.clear();

    for (auto&& [pixel, r, escape_index] :
        run_algorithm_per_pixel<N_algorithm_functor>(res, domain, max_iterations))
    {
        if (N_algorithm_functor::is_trivially_outside(r))
        {
            //grey
            fractal_jet.set_pixel(pixel.x, pixel.y, 100, 100, 100);
        }
        else if (N_algorithm_functor::is_trivially_inside(r))
        {
            //light grey
            fractal_jet.set_pixel(pixel.x, pixel.y, 150, 150, 150);
        }
        // Inside N
        else if (escape_index == max_iterations)
        {
            //black
            fractal_jet.set_pixel(pixel.x, pixel.y, 0, 0, 0);
        }
        else
        {
            //white
            fractal_jet.set_pixel(pixel.x, pixel.y, 255, 255, 255);
        }
    }

    fractal_jet.save_image(pic_path);
}
}