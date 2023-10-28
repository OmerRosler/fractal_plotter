export module frc.algorithms:bandt_base;
import std;
export import :base;


namespace frc
{
export template<typename Derived, typename T, std::size_t N>
class dfs_bandt_algorithm_functor
{
protected:
    std::pmr::memory_resource* memory;

public:
	using dfs_iterator = recursive_tree_dfs_iterator<T, N>;
    using value_t = T;

	dfs_bandt_algorithm_functor(std::pmr::memory_resource* memory) noexcept: memory(memory)
    {
        static_assert(bandt_like_fractal_algorithm<Derived>);
    }

    static constexpr std::size_t approximate_maximal_dynamic_memory(unsigned int max_iterations)
    {
        return (sizeof(T) + sizeof(std::size_t)) * max_iterations + sizeof(T);
    }

    unsigned int max_depth_of_recursive_condition(const T& r,
        unsigned int max_iterations,
        function_holder_t<bool(const T&, unsigned int)>&& recursive_condition)
    {
        Derived& self = *static_cast<Derived*>(this);
        dfs_iterator tree_iterator(Derived::generate_fns_for_tree(r),
            Derived::root(r),
            max_iterations,
            memory);

        //note that the implicit conversion is fine as `max_depth <= max_iterations`
        std::size_t max_depth = 0u;

        // The recursive condition is too computationally complex
        if (max_iterations > Derived::effective_max_iterations(r))
        {
            return max_iterations + 2;
        }

        do
        {
            const auto dpt = tree_iterator.depth();

            //skip subtree if node already satisfy the condition
            if (recursive_condition(*tree_iterator, dpt))
            {
                max_depth = std::max(max_depth, dpt);
                tree_iterator.skip_subtree();
                continue;
            }

            //found a full orbit satisfying the condition
            if (dpt == max_iterations)
            {
                return max_iterations;
            }
            ++tree_iterator;

        } while (tree_iterator.depth() != 1);

        //`max_depth` is the furthest level for which we found a point inside the cut-off sphere
        return max_depth;
    }

    /* This algorithm is a DFS approach to the algorithm described in
    * section 9 of Bandt's paper on M
    * We don't use the BFS approach described in the paper because it takes too much memory
    * (The memory complexity is exponential on `max_iterations`)
    * 
    * Here we have linear memory complexity but the algorithm is less cache friendly
    * as we iterate in DFS
    */
	unsigned int operator()(const T& r, unsigned int max_iterations, long double pixel_size)
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

        const auto accumulation_algorithm_result = max_depth_of_recursive_condition(r,
            max_iterations,
            [& r](const T& t, unsigned int) -> bool
        {
            return Derived::stop_iterating_value(r, t);
        });
        const auto pixel_validation_algorithm_result = 
            max_depth_of_recursive_condition(r,
            max_iterations,
            [&r, &pixel_size](const T& t, unsigned int depth) -> bool
            {
                return Derived::translation_vector_satsifies_bound_for_outside_point(
                    r, 
                    pixel_size, 
                    t, 
                    depth);
            });
        // Definetly outside by 9.4
        if (pixel_validation_algorithm_result != max_iterations)
        {
            // This pixel is very special, we need to investigate if this even can happen
            if (accumulation_algorithm_result == max_iterations)
            {
                return max_iterations + 1;
            }
            return 0;
        }
        
        // The accumulation algorithm decides if the point is inside or out
        return accumulation_algorithm_result;

	}
};
}