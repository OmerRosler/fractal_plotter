module;
#include <cassert>
export module frc.drawing:N_attractor;
import :base;
export import frc.algorithms;
namespace frc
{

double bound_param(frc::r2vec_t param)
{
    auto N_ifs_abs_bound = [](double x)
    {
        assert(std::abs(x) <= 1);
        if (x > 0)
        {
            //maxed where all coefficients are +1
            return x / (1.0 - x);
        }
        else
        {
            //maxed where all even coeffcients are +1 and odd coefficients are -1
            return -x / (1.0 + x);
        }
    };
    auto x_bound = N_ifs_abs_bound(param.x);
    auto y_bound = N_ifs_abs_bound(param.y);
    return std::max(x_bound, y_bound);
}
export void plot_partial_N_attractor(frc::r2vec_t param,
    const std::string& pic_path,
    resolution_t res,
    std::size_t max_iterations)
{
    auto bound = bound_param(param);

    //This is some subset of the attractor to zoom in to
    image_metadata_t meta = { res,
            frc::picture_domain_t{.x{-bound / 1, bound / 1}, .y{-bound / 1,bound / 1} }
    };

    N_attractor_algorithm algo{ param };

    //TODO: Use `std::mdspan` instead of passing the vector
    std::vector frame(meta.res.height + 1,
        std::vector<int>(meta.res.width + 1));

    //fill the frame data with the algorithm result
    MPA_attractor_output_to_frame(meta, max_iterations, algo, frame);

    //plot it
    bitmap_image fractal_jet(meta.res.width, meta.res.height);

    //set all pixels to white
    fractal_jet.clear(255);

    //Fill picture from frame
    for (auto i = 0u; i < meta.res.height; ++i)
    {
        for (auto j = 0u; j < meta.res.width; ++j)
        {
            if (frame[i][j] != 0)
            {
                fractal_jet.set_pixel(i, j, 0, 0, 0);
            }
        }
    }
    //save the image
    fractal_jet.save_image(pic_path);


}

struct attractor_012_algorithm : MPA_algorithm_base_t
{
    r2vec_t param;
    attractor_012_algorithm(r2vec_t p) noexcept :
        MPA_algorithm_base_t(ifs_012_metadata(p)),
        param(p)
    {}
    //Used to initialize base in the constructor
    static std::vector<ifs_map_data_t> ifs_012_metadata(r2vec_t param);
};


auto attractor_012_algorithm::ifs_012_metadata(frc::r2vec_t param) -> std::vector<frc::ifs_map_data_t>
{
    auto f_0 = [param](frc::r2vec_t pt)
    {
        return (pt + 0.0) * param;
    };
    auto f_1 = [param](frc::r2vec_t pt)
    {
        return (pt + 1.0) * param;
    };
    auto f_2 = [param](frc::r2vec_t pt)
    {
        return (pt + 2.0) * param;
    };

    const frc::r2vec_t f_0_fixed_point{ 0.0, 0.0 };
    const frc::r2vec_t f_1_fixed_point = param / (1.0 - param);
    const frc::r2vec_t f_2_fixed_point = 2.0 * param / (1.0 - param);

    std::vector<frc::ifs_map_data_t> ifs;
    ifs.emplace_back(f_0, f_0_fixed_point);
    ifs.emplace_back(f_1, f_1_fixed_point);
    ifs.emplace_back(f_2, f_2_fixed_point);
    return ifs;
}


export void plot_012_attractor(frc::r2vec_t param,
    const std::string& pic_path,
    resolution_t res,
    std::size_t max_iterations)
{

    auto N_ifs_abs_bound = [](double x)
        {
            return 2*x / (1.0 - x);
        };
    auto x_bound = N_ifs_abs_bound(param.x);
    auto y_bound = N_ifs_abs_bound(param.y);
    auto bound = std::max(x_bound, y_bound);

    //This is some subset of the attractor to zoom in to
    image_metadata_t meta = { res,
            frc::picture_domain_t{.x{0, bound}, .y{0, bound} }
    };

    attractor_012_algorithm algo{ param };

    //TODO: Use `std::mdspan` instead of passing the vector
    std::vector frame(meta.res.height + 1,
        std::vector<int>(meta.res.width + 1));

    //fill the frame data with the algorithm result
    MPA_attractor_output_to_frame(meta, max_iterations, algo, frame);

    //plot it
    bitmap_image fractal_jet(meta.res.width, meta.res.height);

    //set all pixels to white
    fractal_jet.clear(255);

    //Fill picture from frame
    for (auto i = 0u; i < meta.res.height; ++i)
    {
        for (auto j = 0u; j < meta.res.width; ++j)
        {
            if (frame[i][j] != 0)
            {
                fractal_jet.set_pixel(i, j, 0, 0, 0);
            }
        }
    }
    auto candidate_xcoordinate = unsigned int (res.width / y_bound);
    auto candidate_ycoordinate = unsigned int(res.height * (1.0 - 1.0 / y_bound));
    //Color the point (1,1) differently
    if (frame[candidate_xcoordinate][candidate_ycoordinate] != 0)
    {
        std::print("Zero inside for \\gamma={}, \\lambda={}", param.x, param.y);
    }
    fractal_jet.set_pixel(candidate_xcoordinate, candidate_ycoordinate, 255, 0, 0);
    //save the image
    fractal_jet.save_image(pic_path);


}

};