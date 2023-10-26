#include "mandlebrot_drawer.hpp"
#include "bandt_drawer.hpp"
#include "N_drawer.hpp"
#include "zoom_in_tool.hpp"
#include "N_attractor_plotting.hpp"
using namespace frc;
int main()
{
    //auto [q_res, q_dom, q_pic_name] = generate_domain_and_resolution(std::complex(0.0, 0.0), 2);
    //draw_mandlebrot(std::format("pics/mandlebrot_{}_depth_{}.bmp", q_pic_name, 100),
    //    {q_res, q_dom}, 100);

    //auto [m_res, m_dom, m_pic_name] = 
    //    generate_domain_and_resolution(std::complex(1/std::sqrt(8), 1 / std::sqrt(8)), 1 / std::sqrt(8));
    //draw_M(std::format("pics/bandt_{}_depth_{}.bmp", m_pic_name, 20),
    //    {m_res, m_dom}, 20);

    //draw_mandlebrot("pics/mandlebrot_debug.bmp",
    //    {resolution_t{ 600, 600 },
    //    picture_domain_t{ .x{-1, 1}, .y{-1, 1 } }},
    //    100);

    /*draw_M("pics/M_debug.bmp",
        {resolution_t{ 600, 600 },
        picture_domain_t{ .x{0, 1.0}, .y{0, 1.0} }}, 50);*/
     
    ////This point was generated by a different program
    //std::complex potential_pt = { 0.602771333745229, 0.7501489829916319 };
    //auto [res, dom, pic_name] = 
    //    generate_domain_and_resolution(potential_pt, 0.01);
    //draw_N("pics/candidate_pt.bmp", { res, dom }, 40);
    //draw_N("pics/N_debug.bmp",
    //    {resolution_t{ 600, 600 },
    //    picture_domain_t{ .x{0.5,1}, .y{0.5,1} }}, 20);

    frc::r2vec_t param = { 1 / 1.35, 1 / 1.55 };
    plot_entire_N_attractor(param,
        "pics/filter_test.bmp",
        frc::resolution_t{ 800, 800 },
        20);

    return 0;
}
