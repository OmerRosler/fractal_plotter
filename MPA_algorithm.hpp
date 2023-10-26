#pragma once
#include <vector>
#include <queue>

#include "utils.hpp"
#include "bitmap_image.hpp"

/*
TODOs: 
2. Organize the code: move the algorithm to a struct with state-machine
3. Add a frame data structure
4. Add zoom-in facility by "adaptive cut" type algorithm
*/
namespace frc
{

struct ifs_map_data_t
{
	function_holder_t<r2vec_t(r2vec_t)> map;
	r2vec_t fixed_point;
};

struct iterated_point
{
    r2vec_t point;
    unsigned int num_of_iterations = 0u;
};


void draw_entire_attractor(
    const std::string& pic_path,
    image_metadata_t meta,
    unsigned int max_iterations,
    std::vector<ifs_map_data_t> ifs);
}