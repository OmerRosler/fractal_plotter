#include <complex>

#include "generator.hpp"
#include "bitmap_image.hpp"
bool are_doubles_equal(double a, double b)
{
    return std::abs(b - a) <= std::numeric_limits<double>::epsilon();
}
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

template<typename I>
std::generator<I> python_xrange(I first, I last, I step = I(1))
{
    do
    {
        co_yield step;
        first += step;
    } while (first != last);
}

template<std::ranges::range Rng1, std::ranges::range Rng2>
std::generator < std::tuple<typename Rng1::value_type, 
    typename Rng2::value_type>> cartesian_product(Rng1&& rng1, Rng2&& rng2)
{
    for (auto&& x : rng1)
    {
        for (auto&& y : rng2)
        {
            co_yield std::tuple{ std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), };
        }
    }
}

struct pixel_coordinates
{
    unsigned int x;
    unsigned int y;
};

struct interval
{
    double start;
    double end;

    double length() const { return end - start; }
};

// The domain is a rectangle as BMP files are rectangles
struct picture_domain
{
    interval x;
    interval y;

    double area() const { return x.length() * y.length(); }
};
/* Yields:
*  - the starting pixel
*  - the magnitude of the iterate when it exceeds 4
*  - the index of escape
*/
std::generator<std::tuple<pixel_coordinates, double, unsigned int>> mandlebrot_pixel_map(unsigned int width, unsigned int height, unsigned int max_iterations, picture_domain domain)
{
    for (unsigned int y : std::ranges::views::iota(0u, height))
    {
        for (unsigned int x : std::ranges::views::iota(0u, width))
        {
            // c runs on the domain [-2,1]x[-1,1] with equal spacing according to the pixels recorded
            std::complex<double> c = 0;
            c.real(domain.x.length() * x / width + domain.x.start);
            c.imag(domain.y.length() * y / height + domain.y.start);

            auto i = iterate_mandlebrot(c, max_iterations);
            co_yield{ {x,y}, std::abs(c), i };
            
        }
    }
}

void draw_mandlebrot(unsigned int width, unsigned int height, unsigned int max_iterations, picture_domain domain)
{
    assert(are_doubles_equal(double(width) / height, domain.x.length() / domain.y.length()));
    bitmap_image fractal_jet(width, height);

    fractal_jet.clear();

    for (auto&& [pixel, escape_mag ,escape_index] : mandlebrot_pixel_map(width, height, max_iterations, domain))
    {
        if (escape_index == max_iterations + 1)
        {
            fractal_jet.set_pixel(pixel.x, pixel.y, 0, 0, 0);
            continue;
        }

        auto index = static_cast<unsigned int>
            (1000.0 * std::log2(1.75 + escape_index - std::log2(std::log2(escape_mag))) / std::log2(max_iterations));

        rgb_t color = jet_colormap[index];

        fractal_jet.set_pixel(pixel.x, pixel.y, color.red, color.green, color.blue);
    }

    fractal_jet.save_image("pics/test20_mandelbrot_set_jet.bmp");
}

int main()
{
   draw_mandlebrot(1200, 800, 1000, { .x{0,1}, .y{-1, -1.0/3} });
   return 0;
}
