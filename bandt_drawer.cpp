#include "bandt_drawer.hpp"

namespace frc
{
void draw_M(
    const std::string& pic_path,
    image_metadata_t meta,
    unsigned int max_iterations)
{
    assert(meta.dom.is_resolution_for_domain(meta.res));
    bitmap_image fractal_jet(meta.res.width, meta.res.height);

    fractal_jet.clear();

    for (auto&& [pixel, r, escape_index] :
        run_algorithm_per_pixel<bandt_algorithm_functor>(meta, max_iterations))
    {
        if (bandt_algorithm_functor::is_trivially_inside(r))
        {
            //grey
            fractal_jet.set_pixel(pixel.x, pixel.y, 100, 100, 100);
        }
        //// An inconclusive pixel
        //else if (escape_index == max_iterations + 1)
        //{
        //    fractal_jet.set_pixel(pixel.x, pixel.y, 0, 0, 255);
        //}
        //// The validation test precision is too high for the machine
        //else if (escape_index == max_iterations + 2)
        //{
        //    fractal_jet.set_pixel(pixel.x, pixel.y, 255, 0, 0);
        //}
        // Inside M
        else if (escape_index == max_iterations)
        {
            //black
            fractal_jet.set_pixel(pixel.x, pixel.y, 0, 0, 0);
        }
        else //non-trivially outside
        {
            //white
            fractal_jet.set_pixel(pixel.x, pixel.y, 255, 255, 255);
        }
    }

    fractal_jet.save_image(pic_path);
}
}