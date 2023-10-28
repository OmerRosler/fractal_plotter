export module frc.drawing:mandelbrot;
import :base;
import frc.algorithm;
namespace frc
{

export color_t mandelbrot_pixel_color(unsigned int max_iterations,
    const std::complex<double>& escape_value,
    unsigned int escape_index);

/*
*  Draws the mandelbrot set at a given resolution in a given domain
*/
export inline void draw_mandelbrot(
    const std::string& pic_path,
    image_metadata_t meta,
    unsigned int max_iterations)
{
    draw_fractal< mandelbrot_algorithm_functor>
        (pic_path, meta, max_iterations, mandelbrot_pixel_color);
}


}