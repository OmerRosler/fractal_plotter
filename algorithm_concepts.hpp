#pragma once
#include <concepts>
#include <array>
#include <memory>

namespace frc
{
    /* A concept that represents an algorithm to decide if a point is inside a fractal.
    * The call operator return type must be convertible to `unsigned int` and the value
    * `max_iterations` represents an inside point, any other value is for outside
    * (it could be an escape index for example).
    *
    * We also require the functor to have two "easily computable" methods to decide if
    * a point is in a trivial part of the fratal or its' complement.
    */
    template<typename F>
    concept fractal_algorithm = requires {
        typename F::value_t;
        requires std::regular_invocable<F, typename F::value_t&, unsigned int>;
    } &&
        requires {
            requires std::constructible_from<F, std::pmr::memory_resource*>;
            requires std::is_convertible_v<
                std::invoke_result_t<F, typename F::value_t&, unsigned int>,
                    unsigned int>;
    }&&
        requires(typename F::value_t r)
    {
        {F::is_trivially_inside(r)} -> std::convertible_to<bool>;
        {F::is_trivially_outside(r)} -> std::convertible_to<bool>;
    };


    template<typename T, std::size_t N>
    class recursive_tree_dfs_iterator;

    /* A concept to represent the algorithm described in Bandt's paper for
    * determining connctedness loci of a linear IFS
    * 
    * There are (so far) two fratals that models this in this library: M and N
    * 
    * The difference between them is the type of the IFS
    * 
    * The customization points of the algorithm are:
    * 1. The recursive generators of the orbit of a given candiate point
    * 2. The cut off radius
    * 3. The root of the orbit for a given candidate
    */
    template<typename F>
    concept bandt_like_fractal_algorithm = fractal_algorithm<F> &&
        requires(const typename F::value_t & r, const typename F::value_t & t) {
        typename F::dfs_iterator;
        {F::dfs_iterator::arity} -> std::convertible_to<std::size_t>;
        typename recursive_tree_dfs_iterator<typename F::value_t, F::dfs_iterator::arity >;
        requires std::convertible_to<typename F::dfs_iterator, 
            recursive_tree_dfs_iterator<typename F::value_t, F::dfs_iterator::arity>>;

            {F::generate_fns_for_tree(r)}->
                std::convertible_to < typename F::dfs_iterator::generator_fns_container_t>;
            {F::stop_iterating_value(r, t)} ->
                std::convertible_to<bool>;
            {F::root(r)} -> std::convertible_to<typename F::value_t>;

    };



}