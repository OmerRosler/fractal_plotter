export module frc.drawing:M;
export import frc.algorithm;
import :base;
namespace frc
{
    
export color_t M_pixel_color(unsigned int max_iterations,
	const std::complex<double>& escape_value,
	unsigned int escape_index);

/*
*  Draws the mandelbrot set at a given resolution in a given domain
*/
export inline void draw_M(
    const std::string& pic_path,
	image_metadata_t meta,
    unsigned int max_iterations)
{
    draw_fractal< bandt_algorithm_functor>
        (pic_path, meta, max_iterations, M_pixel_color);
}



}
