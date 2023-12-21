import frc.algorithms;
import frc.drawing;

using namespace frc;
int main()
{
    //auto [q_res, q_dom, q_pic_name] = generate_domain_and_resolution(std::complex(0.0, 0.0), 2);
    //draw_mandelbrot(std::format("pics/mandelbrot_{}_depth_{}.bmp", q_pic_name, 100),
    //    {q_res, q_dom}, 100);

    //auto [m_res, m_dom, m_pic_name] = 
    //    generate_domain_and_resolution(std::complex(1/std::sqrt(8), 1 / std::sqrt(8)), 1 / std::sqrt(8));
    //draw_M(std::format("pics/bandt_{}_depth_{}.bmp", m_pic_name, 20),
    //    {m_res, m_dom}, 20);

    //draw_mandelbrot("pics/mandelbrot_debug.bmp",
    //    {resolution_t{ 600, 600 },
    //    picture_domain_t{ .x{-1, 1}, .y{-1, 1 } }},
    //    100);

    /*draw_M("pics/M_debug.bmp",
        {resolution_t{ 600, 600 },
        picture_domain_t{ .x{0, 1.0}, .y{0, 1.0} }}, 50);*/
     
    ////This point was generated by a different program
    /*std::complex potential_pt = { 0.602771333745229, 0.7501489829916319 };
    auto [res, dom, pic_name] = 
        generate_domain_and_resolution(potential_pt, 0.01);
    draw_N("pics/candidate_pt.bmp", { res, dom }, 40);*/
    const auto max_iterations = 80;
    //TODO: This is the wrong point and interval
    std::complex potential_pt = { -0.708333, 0.875 };
    const auto length = 0.3334;
    auto [meta, name] = generate_domain_and_resolution(potential_pt, length);
    meta.zoom_in(31, 285, 20);
    meta.zoom_in(280,120, 60);
    draw_N(std::format("pics/new_N__opp_zoom_iter_{}_x_{:10f}_y_{:10f}_length_{:10f}.bmp",
        max_iterations, meta.dom.x.start, meta.dom.y.start, meta.dom.x.length()), meta, max_iterations);
    
    /*draw_N("pics/N_opp_signs_entire_debug", 
        image_metadata_t{ 
            .res={ 600, 600 },
            .dom={ 
                .x={-1,0},
                .y={0,1} 
            } 
        }, max_iterations);*/
    /*const image_metadata_t meta{ 
        resolution_t{ 600, 600 },
        picture_domain_t {.x{0.6646,0.6646 +0.0044}, .y{0.670467,0.670467 +0.0044} } };
    draw_N(std::format("pics/N_zoom_iter_{}_x_{}_y_{}_length_{}.bmp",
        max_iterations, meta.dom.x.start, meta.dom.y.start, meta.dom.x.length()),
        meta, max_iterations);
    */
    /*for (auto i = 1.1; i < 2; i += 0.1)
    {
        for (auto j = i; j < 2; j += 0.1)
        {
            frc::r2vec_t param = { -1 / j, 1 / i };
            const auto max_iterations = 25;
            plot_partial_N_attractor(param,
                std::format("pics/attractors/minus_iter_{}_param_over_{:10f}_m{:10f}.bmp", max_iterations, i, j),
                frc::resolution_t{ 800, 800 },
                max_iterations);
        }
    }*/
    return 0;
}
