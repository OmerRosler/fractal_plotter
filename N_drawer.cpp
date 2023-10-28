module frc.drawing:N;
namespace frc
{
color_t N_pixel_color(unsigned int max_iterations,
    const r2vec_t& r,
    unsigned int escape_index)
{
    if (N_algorithm_functor::is_trivially_outside(r))
    {
        //grey
        return { 100, 100, 100 };
    }
    if (N_algorithm_functor::is_trivially_inside(r))
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

}