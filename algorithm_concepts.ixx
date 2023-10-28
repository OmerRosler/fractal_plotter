export module frc.concepts;
import std;

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
export template<typename F>
concept fractal_algorithm = requires {
    typename F::value_t;
    requires std::regular_invocable<F, typename F::value_t&, unsigned int, long double>;
    } &&
        requires {
            requires std::constructible_from<F, std::pmr::memory_resource*>;
            requires std::is_convertible_v<
                std::invoke_result_t<F, typename F::value_t&, unsigned int, long double>,
                    unsigned int>;
    }&&
        requires(typename F::value_t r)
    {
        {F::is_trivially_inside(r)} -> std::convertible_to<bool>;
        {F::is_trivially_outside(r)} -> std::convertible_to<bool>;
    };

//TODO: Why should forward declarations be exported?
export template<typename T, std::size_t N>
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
* 2. The cut off radius for potential accumulation point
* 3. The root of the orbit for a given candidate
* 4. The lower bound for translation vectors
*    needed to verify a pixel is entirely outside the fractal
*/
export template<typename F>
concept bandt_like_fractal_algorithm = fractal_algorithm<F> &&
    requires(const typename F::value_t & r, const typename F::value_t & t) {
    typename F::dfs_iterator;
    {F::dfs_iterator::arity} -> std::convertible_to<std::size_t>;
    typename recursive_tree_dfs_iterator<typename F::value_t, F::dfs_iterator::arity >;
    std::convertible_to<typename F::dfs_iterator, 
        recursive_tree_dfs_iterator<typename F::value_t, F::dfs_iterator::arity>>;

        {F::generate_fns_for_tree(r)}->
            std::convertible_to < typename F::dfs_iterator::generator_fns_container_t>;

        {F::stop_iterating_value(r, t)} ->
            std::convertible_to<bool>;
        {F::root(r)} -> std::convertible_to<typename F::value_t>;

        {F::translation_vector_satsifies_bound_for_outside_point(r, double(), r, 0u)}  ->
            std::convertible_to<bool>;

};
export struct ifs_map_data_t;
export struct picture_domain_t;
export struct r2vec_t;
/*
* The minimal information required for an IFS attractor plotting
* using the Minimal Plotting algorithm:
* Create a FIFO structure of points definitely inside and apply all maps 
* on them
* 
* There is a modification of the algorithm when we zoom-in -- 
* the ability to skip points if they will never reach the frame.
* This is encapsulated in the second concept
*/
export template<typename Alg>
concept MPA_algorithm_like = requires(Alg alg) {
    {alg.ifs} -> std::ranges::range;
    std::convertible_to<std::ranges::range_value_t<decltype(alg.ifs)>, 
        ifs_map_data_t>;
};

export template<typename Alg>
concept skippable_MPA_algorithm_like = MPA_algorithm_like<Alg> &&
    requires(Alg && alg, 
    picture_domain_t & dom, 
    r2vec_t point, 
    unsigned int num_iterations)
{
    {alg.will_not_be_in_domain(dom, point, num_iterations)} -> 
        std::convertible_to<bool>;
};


}