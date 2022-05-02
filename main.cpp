#include "mandlebrot_drawer.hpp"
#include "bandt_algorithm.hpp"
int main()
{
    /*draw_mandlebrot("pics/mandlebrot.bmp",
        resolution_t{1200, 800},
        picture_domain_t{ .x{0.1,0.4}, .y{-0.1, 0.1} },
        1000);*/
    draw_M("pics/M.bmp",
        resolution_t{ 1200,800 },
        picture_domain_t{ .x{-3, 3}, .y{-2,2} },
        30);
    return 0;
}
