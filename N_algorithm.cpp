#include "N_algorithm.hpp"

void N_algorithm_functor::generate_next_set(r2vec_t r)
{
    const auto ro = std::max(std::abs(r.x), std::abs(r.y));
    const auto cut_off = 1 / (1.0 - ro);

    t_np1->clear();

    for (const auto& v : *t_n)
    {
        const r2vec_t candidates[] = { g0(r,v), g1(r,v), gm1(r,v) };
        for (const auto& cand : candidates)
        {
            if (std::sqrt(cand.x * cand.x + cand.y * cand.y) <= cut_off)
            {
                t_np1->push_back(cand);
            }
        }
    }
}

unsigned int N_algorithm_functor::operator()(r2vec_t r,
    unsigned int max_iterations)
{
    t_n->clear();
    t_np1->clear();

    if (N_algorithm_functor:: is_trivially_outside(r))
    {
        return max_iterations;
    }

    if (N_algorithm_functor::is_trivially_inside(r))
    {
        return 0;
    }
    //Note there is no devision by zero here because these cases are covered by the trivial parts
    t_n->push_back({ 1.0 / r.x, 1.0 / r.y });

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
