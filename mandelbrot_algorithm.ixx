export module frc.algorithm:mandelbrot;
import std;
import :base;

namespace frc
{

/* Takes a complex number c, iterates z^2+c until |z|^2>4 or max_iterations
* It sets `c` the last iterate and returns the index in which the cutoff was passed
*
* If we reach `max_iterations` without the cut-off it returns `max_iterations + 1`
*/
unsigned int iterate_mandelbrot(std::complex<double>& c, unsigned int max_iterations)
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

/* The struct used by the `run_algorithm_per_pixel` functions
*/
export struct mandelbrot_algorithm_functor
{
    using value_t = std::complex<double>;
    mandelbrot_algorithm_functor(std::pmr::memory_resource* rsc = nullptr) noexcept {};

    unsigned int operator()(std::complex<double>& c, 
        unsigned int max_iterations, 
        long double pixel_size) const
    {
        return iterate_mandelbrot(c, max_iterations);
    }

    static bool is_trivially_inside(std::complex<double> r)
    {
        return r == 0.0;
    }

    static bool is_trivially_outside(std::complex<double> r)
    {
        return std::norm(r) >= 4;
    }
};

}