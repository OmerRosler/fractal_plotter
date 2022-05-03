#include "mandlebrot_drawer.hpp"
#include "bandt_algorithm.hpp"
/*
    TODO:
    1. Preallocate the vectors
    2. Parallelize separate pixels
*/
int main()
{
    /*draw_mandlebrot("pics/mandlebrot.bmp",
        resolution_t{1200, 800},
        picture_domain_t{ .x{0.1,0.4}, .y{-0.1, 0.1} },
        1000);*/
    draw_M("pics/M.bmp",
        resolution_t{ 800,800 },
        picture_domain_t{ .x{0, 1/std::sqrt(2)}, .y{-1 / std::sqrt(2), 0}},
        30);
    return 0;
}
