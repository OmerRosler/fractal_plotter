export module frc.algorithm.drawing.N;
import frc.utils;
export import frc.algorithm.N;
import frc.algorithm.drawing;
namespace frc
{

export color_t N_pixel_color(unsigned int max_iterations,
	const r2vec_t& escape_value,
	unsigned int escape_index);

/*
*  Draws the mandlebrot set at a given resolution in a given domain
*/
export inline void draw_N(
    const std::string& pic_path,
	image_metadata_t meta,
    unsigned int max_iterations)
{
    draw_fractal< N_algorithm_functor>
        (pic_path, meta, max_iterations, N_pixel_color);

}

}