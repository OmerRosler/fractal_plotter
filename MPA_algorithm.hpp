#pragma once
#include <vector>
#include <queue>

#include "utils.hpp"
#include "bitmap_image.hpp"

namespace frc
{
/* The minimal amount of data the MPA algorithm requires:
* the IFS maps and their fixed points
*/
struct ifs_map_data_t
{
	function_holder_t<r2vec_t(r2vec_t) const> map;
	r2vec_t fixed_point;

    ifs_map_data_t(function_holder_t<r2vec_t(r2vec_t) const>&& fn, const r2vec_t& fp):
        map(std::move(fn)), fixed_point(fp)
    {
        auto res = map(fixed_point);
        assert(almost_equal(res.x, fixed_point.x) &&
            almost_equal(res.y, fixed_point.y) &&
            "Input point is not fixed under the map");
    }
};

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
struct MPA_algorithm_base_t
{
    std::vector<ifs_map_data_t> ifs;
};

struct cylinder_set_t
{
    using letter_t = decltype(MPA_algorithm_base_t::ifs)::const_iterator;
    using word_t = std::vector<letter_t>;
    word_t word;

    cylinder_set_t() = default;
    cylinder_set_t(const word_t& word) : word(word) {}
    cylinder_set_t(word_t&& word) : word(std::move(word)) {}
    
    std::size_t length() const
    {
        return word.size();
    }

    // apply the word to a point known to be in the attractor
    r2vec_t apply_word(const r2vec_t& point) const
    {
        r2vec_t new_pt = point;
        for (auto let : std::views::reverse(word))
        {
            new_pt = let->map(new_pt);
        }
        return new_pt;
    }


};

/* This is the main MPA algorithm. See the concepts page for the description
*/
template<MPA_algorithm_like Algo>
void MPA_attractor_output_to_frame(
    image_metadata_t meta,
    unsigned int max_iterations,
    Algo&& algorithm,
    std::vector<std::vector<int>>& out_frame,
    std::vector<cylinder_set_t> specific_cylinders)
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
            if (out_frame[coords.first][coords.second] == 0)
            {
                out_frame[coords.first][coords.second] = 1;
            }
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
                    assert(!meta.dom.is_in_range(new_pt) && "Bad condition");
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
        auto first_point = inside_pixels.front();
        // for each cylinder set u_n, calculate u_n w, where w is known to be in the attractor and end up in the frame
        // TODO: Replace with std::views::enumerate when available
        for (auto i = 0u; auto cylinder_set : specific_cylinders)
        {
            if (cylinder_set.length() + first_point.num_of_iterations >= max_iterations)
            {
                i++;
                continue;
            }
            auto point_in_cylinder = cylinder_set.apply_word(first_point.point);
            //Mark the point inside the i-th cylinder set
            if (meta.dom.is_in_range(point_in_cylinder))
            {
                auto coords = meta.pixel_id_from_value(point_in_cylinder.x, point_in_cylinder.y);
                out_frame[coords.first][coords.second] = 2 + i;
            }
            i++;
        }
        inside_pixels.pop();
    }

}

}