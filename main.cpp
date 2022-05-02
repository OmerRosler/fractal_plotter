#include "mandlebrot_drawer.hpp"

int main()
{
    draw_mandlebrot("pics/mandlebrot.bmp",
        resolution_t{1200, 800},
        picture_domain_t{ .x{0.1,0.4}, .y{-0.1, 0.1} },
        1000);
    return 0;
}
