#include "mandlebrot_drawer.hpp"

void draw_mandlebrot(
    const std::string& pic_path,
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations)
{
    assert(domain.is_resolution_for_domain(res));
    bitmap_image fractal_jet(res.width, res.height);

    fractal_jet.clear();

    for (auto&& [pixel, escape_mag, escape_index] :
        run_algorithm_per_pixel<mandlebrot_algorithm_functor>(res, domain, max_iterations))
    {
        // The pixel did NOT escape and is inside the Mandlebrot set
        if (escape_index == max_iterations)
        {
            //black
            fractal_jet.set_pixel(pixel.x, pixel.y, 0, 0, 0);
            continue;
        }

        // Color formula taken from the bitmap library examples, don't know how it works
        auto index = static_cast<unsigned int>
            (1000.0 * std::log2(1.75 + escape_index - std::log2(std::log2(escape_mag))) / std::log2(max_iterations));

        rgb_t color = jet_colormap[index];

        fractal_jet.set_pixel(pixel.x, pixel.y, color.red, color.green, color.blue);
    }

    fractal_jet.save_image(pic_path);
}