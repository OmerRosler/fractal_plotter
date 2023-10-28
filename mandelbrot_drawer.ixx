export module frc.drawing:mandelbrot;
import :base;
import frc.algorithms;
namespace frc
{

export color_t mandelbrot_pixel_color(std::size_t max_iterations,
    const std::complex<double>& escape_value,
    std::size_t escape_index)
{
    // The pixel did NOT escape and is inside the Mandelbrot set
    if (escape_index == max_iterations)
    {
        //black
        return { 0, 0, 0 };
    }

    // Color formula taken from the bitmap library examples, don't know how it works
    auto index = static_cast<std::size_t>
        (1000.0 * std::log2(1.75 + escape_index -
            std::log2(std::log2(std::abs(escape_value)))) / std::log2(max_iterations));

    return jet_colormap[index];

}

/*
*  Draws the mandelbrot set at a given resolution in a given domain
*/
export inline void draw_mandelbrot(
    const std::string& pic_path,
    image_metadata_t meta,
    std::size_t max_iterations)
{
    draw_fractal< mandelbrot_algorithm_functor>
        (pic_path, meta, max_iterations, mandelbrot_pixel_color);
}


}