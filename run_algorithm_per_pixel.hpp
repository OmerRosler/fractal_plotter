#pragma once
#include <memory_resource>

#include "utils.hpp"
namespace frc
{

/* This is a serial algorithm that runs a functor that decides whether a complex point
* is in a given fractal
* 
* Note that in order to parallelize this we need to give up on using generators
* 
* Yields: 
*  - the starting pixel
*  - the magnitude of the value after escape
*  - the index of escape
*/
template<fractal_algorithm IsInFractal> 
    requires std::same_as<typename IsInFractal::value_t, 
    std::complex<double>>
std::generator<std::tuple<pixel_coordinates_t,
    std::complex<double>,
    std::invoke_result_t<IsInFractal, 
        std::complex<double>&, 
        unsigned int,
        long double>
    >>
run_algorithm_per_pixel(
	resolution_t res,
	picture_domain_t domain,
	unsigned int max_iterations)
{
    const auto pixel_size_bound = domain.min_bounding_radius_for_pixel(res);

    // Initialize memory required to run the algorithm
    const auto dynamic_size = caclulate_pre_allocation_buffer_size<IsInFractal>(max_iterations);
    std::pmr::unsynchronized_pool_resource 
        pool(std::pmr::pool_options{ .max_blocks_per_chunk = 0, 
        .largest_required_pool_block = dynamic_size });

    IsInFractal algorithm(&pool);

    for (auto x = 0u; x < res.width; x++)
    {
        for (auto y = 0u; y < res.height; y++)
        {
            // r runs in constant steps across the pixels of the frame
            std::complex<double> r = 0;
            r.real((domain.x.length() * (x+0.5)) / res.width + domain.x.start);
            // y runs in reverse because of the plotting lib
            //TODO: Make sure the sign of 0.5 is the correct one
            r.imag(-domain.y.length() * (y- 0.5) / res.height + domain.y.end);

            // Apply the algorithm, note `r` might change after this line
            auto i = std::invoke(algorithm, r, max_iterations, pixel_size_bound);
            co_yield{ {x,y}, r, i };
        }
    }
}

/* This is a serial algorithm that runs a functor that decides whether a point in R2
* is in a given fractal
*
* Note that in order to parallelize this we need to give up on using generators
*
* Yields:
*  - the starting pixel
*  - the magnitude of the value after escape
*  - the index of escape
*/
template<fractal_algorithm IsInFractal>
    requires std::same_as<typename IsInFractal::value_t,
                            r2vec_t>
std::generator<std::tuple<pixel_coordinates_t,
    r2vec_t,
    std::invoke_result_t<IsInFractal, r2vec_t&, unsigned int, long double>>>
    run_algorithm_per_pixel(
        resolution_t res,
        picture_domain_t domain,
        unsigned int max_iterations)
{
    const auto pixel_size_bound = domain.min_bounding_radius_for_pixel(res);
    
    // Initialize memory required to run the algorithm
    const auto dynamic_size = caclulate_pre_allocation_buffer_size<IsInFractal>(max_iterations);
    std::pmr::unsynchronized_pool_resource
        pool(std::pmr::pool_options{ .max_blocks_per_chunk = 0,
        .largest_required_pool_block = dynamic_size });

    IsInFractal algorithm(&pool);

    for (auto x = 0u; x < res.width; x++)
    {
        for (auto y = 0u; y < res.height; y++)
        {
            // r runs in constant steps across the pixels of the frame
            // y runs in reverse because of the plotting lib
            //TODO: Make sure the sign of 0.5 is the correct one
            r2vec_t r = {
            ((domain.x.length() * (x+0.5)) / res.width + domain.x.start),
            (-domain.y.length() * (y - 0.5) / res.height + domain.y.end) };

            // Apply the algorithm, note `r` might change after this line
            auto i = std::invoke(algorithm, r, max_iterations, pixel_size_bound);
            co_yield{ {x,y}, r, i };
        }
    }
}
}