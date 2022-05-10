#pragma once

/* This algorithm to draw N is almost identical to Bandth algorithm to draw M
* Chapter 9 of the paper works almost exactly with the following modifications:
* 
* g_i(x,y) = ((x+i)/a, (y+i)/b)
* 
* where (a,b) is the candidate point
* 
* ro = max{|a|,|b|}
*/

#include <vector>
#include <ranges>
#include <cmath>
#include <array>
#include <functional>

#include "utils.hpp"
#include "recursive_tree_dfs_iterator.hpp"


class N_algorithm_functor
{
	
	using dfs_iterator = recursive_tree_dfs_iterator<r2vec_t, 3>;

public:

	struct memory_layout_t {};


	N_algorithm_functor(memory_layout_t& memory) {}

	unsigned int operator()(r2vec_t r, unsigned int max_iterations);

	static bool is_trivially_inside(r2vec_t r)
	{
		return std::abs(r.x * r.y) > 0.5;
	}
	static bool is_trivially_outside(r2vec_t r)
	{
		return (r.x >= 1.0 || r.x <= -1.0) ||
			(r.y >= 1.0 || r.y <= -1.0) ||
			(almost_equal(r.x, 0.0) && !almost_equal(r.y, 0.0)) ||
			(almost_equal(r.y, 0.0) && !almost_equal(r.x, 0.0));
	}
private:
	static r2vec_t g0(r2vec_t r, r2vec_t t)
	{
		return t / r;
	}

	static r2vec_t g1(r2vec_t r, r2vec_t t)
	{
		return (t + r2vec_t{ 1.0, 1.0 }) / r;
	}

	static r2vec_t gm1(r2vec_t r, r2vec_t t)
	{
		return (t - r2vec_t{ 1.0, 1.0 }) / r;

	}

	std::array<dfs_iterator::generator_t, 3> generate_fns_for_tree(const r2vec_t& r)
	{
		return { std::bind_front(g0, r), std::bind_front(g1, r), std::bind_front(gm1, r) };
	}

};

