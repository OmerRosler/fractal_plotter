#include "mandlebrot_drawer.hpp"
#include "bandt_drawer.hpp"
#include "N_drawer.hpp"
#include "zoom_in_tool.hpp"

int main()
{
    auto [q_res, q_dom, q_pic_name] = generate_domain_and_resolution(std::complex(0.0, 0.0), 2);
    draw_mandlebrot(std::format("pics/mandlebrot_{}_depth_{}.bmp", q_pic_name, 100),
        q_res, q_dom, 100);

    auto [m_res, m_dom, m_pic_name] = 
        generate_domain_and_resolution(std::complex(1/std::sqrt(8), 1 / std::sqrt(8)), 1 / std::sqrt(8));
    draw_M(std::format("pics/bandt_{}_depth_{}.bmp", m_pic_name, 20),
        m_res, m_dom, 20);

    draw_N("pics/N_debug.bmp",
        resolution_t{ 600, 600 },
        picture_domain_t{ .x{0.5,1}, .y{0.5,1} }, 30);

    return 0;
}
