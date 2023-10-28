module;
#include <assert.h>
export module frc.algorithms:attractor_MPA;
import :base;
import bitmap_image;

namespace frc
{

/* The information on the points used by the algorithm
* This may change in the future if we want infinite zoom-in
*/
struct iterated_point
{
    r2vec_t point;
    unsigned int num_of_iterations = 0u;
};

/* The basic amount of information needed for the MPA algorithm
*/
export struct MPA_algorithm_base_t
{
    std::vector<ifs_map_data_t> ifs;
};

/* This is the main MPA algorithm. See the concepts page for the description
*/
export template<MPA_algorithm_like Algo>
void MPA_attractor_output_to_frame(
    image_metadata_t meta,
    unsigned int max_iterations,
    Algo&& algorithm,
    std::vector<std::vector<int>>& out_frame)
    //TODO: Output the algorithm result to an `std::mdspan` type for the frame once available
//std::mdspan<int, std::dextents<int,2>> out_frame)
{
    assert(meta.dom.is_resolution_for_domain(meta.res));

    std::queue<iterated_point> inside_pixels;
    //Fill the queue with the fixed points of the maps (which are definetly in the attractor)
    for (auto&& [_, fp] : algorithm.ifs)
    {
        inside_pixels.push({ fp });
    }
    while (!inside_pixels.empty())
    {
        //Extract point known to be inside the attractor
        auto [point, num_iterations] = inside_pixels.front();

        auto coords = meta.pixel_id_from_value(point.x, point.y);


        //Mark the point inside
        if (meta.dom.is_in_range(point))
        {
            out_frame[coords.first][coords.second] = 1;
        }

        // Early breakout if needed
        if (num_iterations == max_iterations)
        {
            inside_pixels.pop();
            continue;
        }

        //If skipping is available, use it with the domain given
        if constexpr (skippable_MPA_algorithm_like<Algo>)
        {
            //Skip points that will never not the input domain 
            // given the number of left over iterations
            if (algorithm.will_not_be_in_domain(meta.dom, 
                point, 
                max_iterations - num_iterations))
            {
#ifndef NDEBUG
                for (auto&& [ifs_map, _] : algorithm.ifs)
                {
                    auto new_pt = ifs_map(point);
                    assert(!meta.dom.is_in_range(new_pt));
                }
#endif
                inside_pixels.pop();
                continue;
            }
        }

        //Apply all maps to the point and add them to the queue
        for (auto&& [ifs_map, _] : algorithm.ifs)
        {
            auto new_point = ifs_map(point);

            inside_pixels.emplace(new_point, num_iterations + 1);
        }

        inside_pixels.pop();
    }

}

}