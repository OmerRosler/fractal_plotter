import std.core;

import bitmap_image;


void draw_mandlebrot()
{
    constexpr unsigned int fractal_width  = 1200;
    constexpr unsigned int fractal_height =  800;

    bitmap_image fractal_jet  (fractal_width,fractal_height);

    fractal_jet  .clear();

    std::complex<double> c = 0;

    const unsigned int max_iterations = 1000;

    for (unsigned int y = 0; y < fractal_height; ++y)
    {
        for (unsigned int x = 0; x < fractal_width; ++x)
        {
            c.real(1.5 * (2.0 * x / fractal_width - 1.0) - 0.5);
            c.imag(2.0 * y / fractal_height - 1.0);

            std::complex<double> next = 0;
            std::complex<double> prev = 0;

            for (unsigned int i = 0; i < max_iterations; i++)
            {
                prev = next;
                next = prev * prev + c;

                const auto z_norm = std::norm(next);

                //if the norm exceeds 4 then `next` is not in the set and we find the color according to `i`
                if (z_norm > 4)
                {
                    const auto z_mag = std::sqrt(z_norm);

                    auto index = static_cast<unsigned int>
                    (1000.0 * std::log2(1.75 + i - std::log2(std::log2(z_mag))) / std::log2(max_iterations));

                    rgb_t color = jet_colormap[index];

                    fractal_jet.set_pixel(x, y, color.red, color.green, color.blue);

                }
            }
        }
    }

    fractal_jet.save_image("pics/test20_mandelbrot_set_jet.bmp"  );

  
}

int main()
{
   draw_mandlebrot();
   return 0;
}
