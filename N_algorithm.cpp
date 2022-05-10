#include "N_algorithm.hpp"

void N_algorithm_functor::dfs_iterator::kill_twos()
{
    //find next "avilable" index
    while ((depth() != 1) && (previous_ids.back() == 2))
    {
        previous_ids.pop_back();
        previous_values.pop_back();
    }
}

auto N_algorithm_functor::dfs_iterator::skip_subtree() -> dfs_iterator
{
    //go back up the tree until there is an avilable borther node
    kill_twos();
    // went too far, reached the end
    if (depth() == 1)
    {
        return null();
    }

    //change the position vector
    previous_ids.back()++;
    //remove the old brother from the back of the tree
    previous_values.pop_back();
    // calculate the new brother value
    const r2vec_t last_value = previous_values.back();
    previous_values.emplace_back(next_fns[previous_ids.back()](r, last_value));

    return *this;
}

auto N_algorithm_functor::dfs_iterator::advance_dfs() -> dfs_iterator
{
    // if we are at a leaf, move sideways
    if (depth() == max_depth)
    {
        return skip_subtree();
    }
    else
    {
        // add 0 to the position vector and generate a new node
        previous_ids.emplace_back(0);
        const r2vec_t last_value = previous_values.back();
        previous_values.emplace_back(next_fns[0](r, last_value));
    }
    return *this;
}

unsigned int N_algorithm_functor::operator()(r2vec_t r,
    unsigned int max_iterations)
{
    if (N_algorithm_functor:: is_trivially_outside(r))
    {
        return 0;
    }

    if (N_algorithm_functor::is_trivially_inside(r))
    {
        return max_iterations;
    }

    tree_iterator.reset(r, { 1.0 / r.x, 1.0 / r.y }, max_iterations);

    //note that the implicit conversion is fine as `max_depth <= max_iterations`
    std::size_t max_depth = 0u;

    const auto ro = std::max(std::abs(r.x), std::abs(r.y));
    const auto cut_off = 1 / (1.0 - ro);

    do
    {
        const auto dpt = tree_iterator.depth();

        //Go back if point is outside
        if (tree_iterator->abs() > cut_off)
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
