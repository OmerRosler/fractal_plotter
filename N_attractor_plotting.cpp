// attractor_plotting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "N_attractor_plotting.hpp"

namespace frc
{

auto N_attractor_algorithm::N_ifs_metadata(frc::r2vec_t param) -> std::vector<frc::ifs_map_data_t>
{
    auto fplus = [param](frc::r2vec_t pt)
    {
        return (pt + 1.0) * param;
    };
    auto fminus = [param](frc::r2vec_t pt)
    {
        return (pt - 1.0) * param;
    };

    const frc::r2vec_t fplus_fixed_point = param / (1.0 - param);
    const frc::r2vec_t fminus_fixed_point = -param / (1.0 - param);

    std::vector<frc::ifs_map_data_t> ifs;
    ifs.emplace_back(fplus, fplus_fixed_point);
    ifs.emplace_back(fminus, fminus_fixed_point);
    return ifs;
}

void plot_partial_N_attractor(frc::r2vec_t param, 
    const std::string& pic_path,
    resolution_t res,
    unsigned int max_iterations)
{
    N_attractor_algorithm algo{ param };

    // specific cylinder sets to color differently
    const cylinder_set_t::letter_t p = algo.ifs.begin();
    const cylinder_set_t::letter_t m = algo.ifs.begin() + 1;
    // these two cylinder set were found numerically to contain a trap
    const cylinder_set_t u10 = { { m,p,p,p,p,m,m,m,m,m } };
    const cylinder_set_t v10 = { { p,m,m,m,p,p,p,p,p,p } };

    // find the region to zoom around

    // relevant points
    const auto m_infty = m->fixed_point;
    const auto p_infty = p->fixed_point;
    const auto u_10_m_infty = u10.apply_word(m_infty);
    const auto v_10_m_infty = v10.apply_word(m_infty);
    const auto u_10_p_infty = u10.apply_word(p_infty);
    const auto v_10_p_infty = v10.apply_word(p_infty);

    double y_dist = (v_10_p_infty.y - u_10_m_infty.y);
    // as the cylinder set is almost vertical, we just take the Delta y
    // and add something around it so it is in the center of the image
    // we can change this parameter as we like
    int y_sorround = 1.0/res.ratio() - 1;
    // the x length is determined from the resolution
    int x_sorround = int(1.0/ ((1 + 1.0 / y_sorround) * res.ratio()));
    image_metadata_t meta = { res,
            frc::picture_domain_t{
            .x{u_10_p_infty.x - y_dist/(2* x_sorround), u_10_p_infty.x+ y_dist/ (2 * x_sorround)},
            .y{u_10_m_infty.y - y_dist/(2* y_sorround), v_10_p_infty.y  + y_dist / (2 * y_sorround)} }
    };
    
    //TODO: Use `std::mdspan` instead of passing the vector
    std::vector frame(meta.res.width + 1,
        std::vector<int>(meta.res.height + 1));
    //fill the frame data with the algorithm result
    MPA_attractor_output_to_frame(meta, max_iterations, algo, frame, {
        u10, v10 });

    //plot it
    bitmap_image fractal_jet(meta.res.width, meta.res.height);

    //set all pixels to white
    fractal_jet.clear(255);

    //Fill picture from frame
    for (int i = 0; i < meta.res.width; ++i)
    {
        for (int j = 0; j < meta.res.height; ++j)
        {
            if (frame[i][j] == 1)
            {
                // black
                fractal_jet.set_pixel(i, j, 0, 0, 0);
            }
            else if (frame[i][j] == 2)
            {
                // color 1
                fractal_jet.set_pixel(i, j, 255, 0, 0);
            }
            else if (frame[i][j] == 3)
            {
                // color 2
                fractal_jet.set_pixel(i, j, 0, 0, 255);
            }
        }
    }
    //save the image
    fractal_jet.save_image(pic_path);

}
}
