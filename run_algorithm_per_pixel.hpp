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
template<complex_fractal_algorithm IsInFractal>
std::generator<std::tuple<pixel_coordinates_t,
    std::complex<double>,
    std::invoke_result_t<IsInFractal, std::complex<double>&, unsigned int>>>
run_algorithm_per_pixel(
	resolution_t res,
	picture_domain_t domain,
	unsigned int max_iterations)
{
    // Initialize memory required to run the algorithm
    const auto dynamic_size = caclulate_pre_allocation_buffer_size<IsInFractal>(max_iterations);
    std::pmr::monotonic_buffer_resource pool{ dynamic_size };

    IsInFractal algorithm(&pool);

    for (auto x = 0u; x < res.width; x++)
    {
        for (auto y = 0u; y < res.height; y++)
        {
            // r runs in constant steps across the pixels of the frame
            std::complex<double> r = 0;
            r.real((domain.x.length() * x) / res.width + domain.x.start);
            // y runs in reverse because of the plotting lib
            r.imag(-domain.y.length() * y / res.height + domain.y.end);

            // Apply the algorithm, note `r` might change after this line
            auto i = std::invoke(algorithm, r, max_iterations);
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
template<r2_fractal_algorithm IsInFractal>
std::generator<std::tuple<pixel_coordinates_t,
    r2vec_t,
    std::invoke_result_t<IsInFractal, r2vec_t&, unsigned int>>>
    run_algorithm_per_pixel(
        resolution_t res,
        picture_domain_t domain,
        unsigned int max_iterations)
{
    // Initialize memory required to run the algorithm
    const auto dynamic_size = caclulate_pre_allocation_buffer_size<IsInFractal>(max_iterations);
    std::pmr::monotonic_buffer_resource pool{ dynamic_size };

    IsInFractal algorithm(&pool);

    for (auto x = 0u; x < res.width; x++)
    {
        for (auto y = 0u; y < res.height; y++)
        {
            // r runs in constant steps across the pixels of the frame
            // y runs in reverse because of the plotting lib
            r2vec_t r = {
            ((domain.x.length() * x) / res.width + domain.x.start),
            (-domain.y.length() * y / res.height + domain.y.end) };

            // Apply the algorithm, note `r` might change after this line
            auto i = std::invoke(algorithm, r, max_iterations);
            co_yield{ {x,y}, r, i };
        }
    }
}
}