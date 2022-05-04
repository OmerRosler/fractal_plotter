#pragma once

#include "utils.hpp"


/* Takes a complex number c, iterates z^2+c until |z|^2>4 or max_iterations
* It sets `c` the last iterate and returns the index in which the cutoff was passed
*
* If we reach `max_iterations` without the cut-off it returns `max_iterations + 1`
*/
unsigned int iterate_mandlebrot(std::complex<double>& c, unsigned int max_iterations);

struct mandlebrot_algorithm_functor
{
    struct memory_layout_t {};
    mandlebrot_algorithm_functor(memory_layout_t&) noexcept {};
    unsigned int operator()(std::complex<double>& c, unsigned int max_iterations) const
    {
        return iterate_mandlebrot(c, max_iterations);
    }
};