#include "mandlebrot_drawer.hpp"
#include "bandt_algorithm.hpp"
/*
    TODO:
    1. Preallocate the vectors
    2. Parallelize separate pixels
*/
int main()
{
    //draw_mandlebrot("pics/mandlebrot.bmp",
    //    resolution_t{800, 800},
    //    picture_domain_t{ .x{0,2}, .y{-1.5, 0.5} },
    //    100);
    draw_M("pics/M.bmp",
        resolution_t{ 800,800 },
        picture_domain_t{ .x{0, 1/std::sqrt(2)}, .y{0, 1 / std::sqrt(2)}},
        20);
    return 0;
}
