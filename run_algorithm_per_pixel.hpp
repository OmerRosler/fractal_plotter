#pragma once
#include "utils.hpp"
/* A concept that represents an algorithm to decide if a point is inside a fractal.
* The call operator return type must be convertible to `unsigned int` and the value
* `max_iterations` represents an inside point, any other value is for outside
* (it could be an escape index for example).
*/
template<typename F>
concept fractal_algorithm = std::regular_invocable<F, std::complex<double>&, unsigned int> &&
    requires {
    typename F::memory_layout_t;
    requires std::constructible_from<F, typename F::memory_layout_t&>;
    requires std::is_convertible_v<
        std::invoke_result_t<F, std::complex<double>&, unsigned int>,
        unsigned int>;
};
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
std::generator<std::tuple<pixel_coordinates_t,
    double, 
    std::invoke_result_t<IsInFractal, std::complex<double>&, unsigned int>>>
run_algorithm_per_pixel(
	resolution_t res,
	picture_domain_t domain,
	unsigned int max_iterations)
{
    // Initialize memory required to run the algorithm
    typename IsInFractal::memory_layout_t alg_memory{};

    IsInFractal algorithm(alg_memory);

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
            co_yield{ {x,y}, std::abs(r), i };
        }
    }
}
