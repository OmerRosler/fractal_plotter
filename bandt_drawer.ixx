export module frc.drawing:M;
export import frc.algorithms;
import :base;
namespace frc
{
    
export color_t M_pixel_color(unsigned int max_iterations,
	const std::complex<double>& escape_value,
	unsigned int escape_index) 
{
    if (bandt_algorithm_functor::is_trivially_inside(escape_value))
    {
        //grey
        return{ 100, 100, 100 };
    }
    //// An inconclusive pixel
    //else if (escape_index == max_iterations + 1)
    //{
    //    fractal_jet.set_pixel(pixel.x, pixel.y, 0, 0, 255);
    //}
    //// The validation test precision is too high for the machine
    //else if (escape_index == max_iterations + 2)
    //{
    //    fractal_jet.set_pixel(pixel.x, pixel.y, 255, 0, 0);
    //}

    // Inside M
    else if (escape_index == max_iterations)
    {
        //black
        return{ 0, 0, 0 };
    }
    else //non-trivially outside
    {
        //white
        return{ 255, 255, 255 };
    }
}

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
