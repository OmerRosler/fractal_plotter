#include "mandlebrot_algorithm.hpp"

namespace frc
{
unsigned int iterate_mandlebrot(std::complex<double>& c, unsigned int max_iterations)
{
    std::complex<double> next = 0;
    std::complex<double> prev = 0;

    for (unsigned int i = 0; i < max_iterations; i++)
    {
        prev = next;
        next = prev * prev + c;

        //if the norm exceeds 4 then `next` is not in the set
        if (std::norm(next) > 4)
        {
            c = next;
            return i;
        }

    }
    return max_iterations;
}
}