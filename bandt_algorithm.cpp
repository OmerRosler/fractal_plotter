#include "bandt_algorithm.hpp"

unsigned int bandt_algorithm_functor::operator()(std::complex<double> r, 
    unsigned int max_iterations)
{
    if (is_trivially_inside(r))
    {
        return max_iterations;
    }
    if (is_trivially_outside(r))
    {
        return 0;
    }

    dfs_iterator tree_iterator(generate_fns_for_tree(r),
        1.0/r,
        max_iterations);

    //note that the implicit conversion is fine as `max_depth <= max_iterations`
    std::size_t max_depth = 0u;

    const auto ro = std::abs(r);
    const auto cut_off = 1 / (1.0 - ro);

    do
    {
        const auto dpt = tree_iterator.depth();

        //Go back if point is outside
        if (std::abs(*tree_iterator) > cut_off)
        {
            max_depth = std::max(max_depth, dpt);
            tree_iterator.skip_subtree();
            continue;
        }
        //found accumolation point, stop early
        if (dpt == max_iterations)
        {
            return max_iterations;
        }
        ++tree_iterator;

    } while (tree_iterator.depth() != 1);

    //`max_depth` is the furthest N for which we found a point inside the cut-off sphere
    return max_depth;
}
