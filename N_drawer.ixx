export module frc.drawing:N;
import :base;
export import frc.algorithms;
namespace frc
{

export color_t N_pixel_color(std::size_t max_iterations,
	const r2vec_t& escape_value,
    std::size_t escape_index)
{
    if (N_algorithm_functor::is_trivially_outside(escape_value))
    {
        //grey
        return { 100, 100, 100 };
    }
    if (N_algorithm_functor::is_trivially_inside(escape_value))
    {
        //light grey
        return { 150, 150, 150 };
    }
    // Inside N
    if (escape_index == max_iterations)
    {
        //black
        return { 0, 0, 0 };
    }
    else
    {
        //white
        return { 255, 255, 255 };
    }
}

/*
*  Draws the mandelbrot set at a given resolution in a given domain
*/
export inline void draw_N(
    const std::string& pic_path,
	image_metadata_t meta,
    std::size_t max_iterations)
{
    draw_fractal< N_algorithm_functor>
        (pic_path, meta, max_iterations, N_pixel_color);

}

}