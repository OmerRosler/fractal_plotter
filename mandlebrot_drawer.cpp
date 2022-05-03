#include "mandlebrot_drawer.hpp"

unsigned int iterate_mandlebrot(std::complex<double>& c, unsigned int max_iterations)
{
    std::complex<double> next = 0;
    std::complex<double> prev = 0;

    for (unsigned int i = 0; i < max_iterations; i++)
    {
        prev = next;
        next = prev * prev + c;

        //if the norm exceeds 4 then `next` is not in the set
        if (std::norm(next) > 4)
        {
            c = next;
            return i;
        }

    }
    return max_iterations + 1;
}

/* Yields:
*  - the starting pixel
*  - the magnitude of the iterate when it exceeds 4
*  - the index of escape
*/
std::generator<std::tuple<pixel_coordinates, double, unsigned int>>
mandlebrot_pixel_map(
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations)
{
    for (unsigned int y : std::ranges::views::iota(0u, res.height))
    {
        for (unsigned int x : std::ranges::views::iota(0u, res.width))
        {
            std::complex<double> c = 0;
            c.real(domain.x.length() * x / res.width + domain.x.start);
            c.imag(-domain.y.length() * y / res.height + domain.y.end);

            auto i = iterate_mandlebrot(c, max_iterations);
            co_yield{ {x,y}, std::abs(c), i };

        }
    }
}

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
        mandlebrot_pixel_map(res, domain, max_iterations))
    {
        // The pixel did NOT escape and is inside the Mandlebrot set
        if (escape_index == max_iterations + 1)
        {
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