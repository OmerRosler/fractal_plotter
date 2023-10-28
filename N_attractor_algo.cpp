module frc.algorithms:N_attractor_based_on_MPA;
import std;
import bitmap_image;
namespace frc
{

auto N_attractor_algorithm::N_ifs_metadata(frc::r2vec_t param) -> std::vector<frc::ifs_map_data_t>
{
    auto f_plus = [param](frc::r2vec_t pt)
    {
        return (pt + 1.0) * param;
    };
    auto f_minus = [param](frc::r2vec_t pt)
    {
        return (pt - 1.0) * param;
    };

    const frc::r2vec_t f_plus_fixed_point = param / (1.0 - param);
    const frc::r2vec_t f_minus_fixed_point = -param / (1.0 - param);

    std::vector<frc::ifs_map_data_t> ifs;
    ifs.emplace_back(f_plus, f_plus_fixed_point);
    ifs.emplace_back(f_minus, f_minus_fixed_point);
    return ifs;
}

}
