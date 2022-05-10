#pragma once

#include "utils.hpp"
#include "recursive_tree_dfs_iterator.hpp"

namespace frc
{
template<typename Derived, typename T, std::size_t N>
class dfs_bandt_algorithm_functor
{
protected:
    std::pmr::memory_resource* memory;

public:
	using dfs_iterator = recursive_tree_dfs_iterator<T, N>;

	dfs_bandt_algorithm_functor(std::pmr::memory_resource* memory) noexcept: memory(memory)
    {
        static_assert(bandt_like_fractal_algorithm<Derived, T, N>);
    }

    static constexpr std::size_t approximate_maximal_dynamic_memory(unsigned int max_iterations)
    {
        return (sizeof(T) + sizeof(std::size_t)) * max_iterations + sizeof(T);
    }

    /* This algorithm is a DFS approach to the algorithm described in
    * section 9 of Bandt's paper on M
    * We don't use the BFS approach described in the paper because it takes too much memory
    * (The memory complexity is exponential on `max_iterations`)
    * 
    * Here we have linear memory complexity but the algorithm is less cache friendly
    * as we iterate in DFS
    */
	unsigned int operator()(const T& r, unsigned int max_iterations)
	{
        Derived& self = *static_cast<Derived*>(this);
        if (self.is_trivially_inside(r))
        {
            return max_iterations;
        }
        if (self.is_trivially_outside(r))
        {
            return 0;
        }

        dfs_iterator tree_iterator(Derived::generate_fns_for_tree(r),
            Derived::root(r),
            max_iterations,
            memory);

        const auto stop_condition = [&r](const T& t) -> bool
        {
            return Derived::stop_iterating_value(r, t);
        };

        //note that the implicit conversion is fine as `max_depth <= max_iterations`
        std::size_t max_depth = 0u;

        do
        {
            const auto dpt = tree_iterator.depth();

            //Go back if point is outside
            if (stop_condition(*tree_iterator))
            {
                max_depth = std::max(max_depth, dpt);
                tree_iterator.skip_subtree();
                continue;
            }

            //found accumulation point up to the given precision, stop early
            if (dpt == max_iterations)
            {
                return max_iterations;
            }
            ++tree_iterator;

        } while (tree_iterator.depth() != 1);

        //`max_depth` is the furthest level for which we found a point inside the cut-off sphere
        return max_depth;
	}


};
}