#include "bandt_algorithm.hpp"

void bandt_algorithm_functor::generate_next_set(std::complex<double> r)
{
    const auto ro = std::abs(r);
    const auto cut_off = 1 / (1.0 - ro);

    t_np1->clear();

    for (const auto& v : *t_n)
    {
        const std::complex<double> candidates[] = { g0(r,v), g1(r,v), gm1(r,v) };
        for (const auto& cand : candidates)
        {
            if (std::abs(cand) <= cut_off)
            {
                t_np1->push_back(cand);
            }
        }
    }
}

unsigned int bandt_algorithm_functor::operator()(std::complex<double> r, 
    unsigned int max_iterations)
{
    t_n->clear();
    t_np1->clear();

    if (is_trivially_inside(r))
    {
        return max_iterations;
    }
    if (is_trivially_outside(r))
    {
        return 0;
    }

    t_n->push_back(1.0 / r);

    for (auto i = 0u; i < max_iterations; i++)
    {
        if (t_n->empty())
        {
            return i;
        }
        generate_next_set(r);
        std::swap(t_n, t_np1);
    }
    return max_iterations;
}
