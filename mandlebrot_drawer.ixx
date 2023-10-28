export module frc.algorithm.drawing.mandlebrot;
import frc.utils;
import frc.algorithm.mandlebrot;
import frc.algorithm.drawing;
namespace frc
{

export color_t mandlebrot_pixel_color(unsigned int max_iterations,
    const std::complex<double>& escape_value,
    unsigned int escape_index);

/*
*  Draws the mandlebrot set at a given resolution in a given domain
*/
export inline void draw_mandlebrot(
    const std::string& pic_path,
    image_metadata_t meta,
    unsigned int max_iterations)
{
    draw_fractal< mandlebrot_algorithm_functor>
        (pic_path, meta, max_iterations, mandlebrot_pixel_color);
}


}