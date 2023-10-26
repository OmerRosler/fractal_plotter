// attractor_plotting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "N_attractor_plotting.hpp"

namespace frc
{

auto N_ifs_metadata(frc::r2vec_t param) -> std::vector<frc::ifs_map_data_t>
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

void plot_entire_N_attractor(frc::r2vec_t param, 
    const std::string& pic_path,
    resolution_t res,
    unsigned int max_iterations)
{
    auto ifs = N_ifs_metadata(param);   

    // We draw the entire IFS here, this is why the bound is calculated as such
    // Note this calculation of the bound is NOT universal and only works for our IFS
    auto N_ifs_abs_bound = [](double x)
    {
        return x / (1.0 - x);
    };
    auto x_bound = N_ifs_abs_bound(param.x);
    auto y_bound = N_ifs_abs_bound(param.y);
    auto bound = std::max(x_bound,y_bound);

    frc::draw_entire_attractor(
        pic_path,
        {   res,
            frc::picture_domain_t{ .x{-bound, bound}, .y{-bound,bound} }
        }, 
        20, 
        std::move(ifs));
    
}
}
