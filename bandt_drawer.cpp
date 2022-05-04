#include "bandt_drawer.hpp"


void draw_M(
    const std::string& pic_path,
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations)
{
    assert(domain.is_resolution_for_domain(res));
    bitmap_image fractal_jet(res.width, res.height);

    fractal_jet.clear();

    for (auto&& [pixel, magnitude, escape_index] :
        run_algorithm_per_pixel<bandt_algorithm_functor>(res, domain, max_iterations))
    {
        // Outside M
        if (escape_index == max_iterations)
        {
            // trivially outside
            if (magnitude > 1.0 / std::sqrt(2))
            {
                //grey
                fractal_jet.set_pixel(pixel.x, pixel.y, 100, 100, 100);
            }
            else
            {
                //black
                fractal_jet.set_pixel(pixel.x, pixel.y, 0, 0, 0);
            }
        }
        else
        {
            //white
            fractal_jet.set_pixel(pixel.x, pixel.y, 255, 255, 255);
        }
    }

    fractal_jet.save_image(pic_path);
}