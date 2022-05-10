#pragma once
#include <concepts>
#include <array>
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
    template<typename F, typename ValueT>
    concept fractal_algorithm = std::regular_invocable<F, ValueT&, unsigned int> &&
        requires {
        typename F::memory_layout_t;
            requires std::constructible_from<F, typename F::memory_layout_t&>;
            requires std::is_convertible_v<
                std::invoke_result_t<F, ValueT&, unsigned int>,
                    unsigned int>;
    }&&
        requires(ValueT r)
    {
        {F::is_trivially_inside(r)} -> std::convertible_to<bool>;
        {F::is_trivially_outside(r)} -> std::convertible_to<bool>;
    };

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
    template<typename F, typename ValueT, std::size_t N>
    concept bandt_like_fractal_algorithm = fractal_algorithm<F, ValueT> &&
        requires(const ValueT & r, const ValueT & t) {

            {F::generate_fns_for_tree(r)}->
                std::convertible_to < std::array<typename F::dfs_iterator::generator_t, N>>;
            {F::stop_iterating_value(r, t)} ->
                std::convertible_to<bool>;
            {F::root(r)} -> std::convertible_to<ValueT>;

    };



}