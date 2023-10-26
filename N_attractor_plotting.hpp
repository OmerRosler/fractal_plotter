#pragma once

#include "bitmap_image.hpp"
#include "utils.hpp"
#include "MPA_algorithm.hpp"


namespace frc
{

std::vector<frc::ifs_map_data_t> N_ifs_metadata(frc::r2vec_t param);

void plot_entire_N_attractor(frc::r2vec_t param,
    const std::string& pic_path,
    resolution_t res,
    unsigned int max_iterations);

}