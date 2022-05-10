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
#include "dfs_bandt_algorithm.hpp"

namespace frc
{
class N_algorithm_functor : public dfs_bandt_algorithm_functor<N_algorithm_functor, r2vec_t, 3>
{
	using base_t = dfs_bandt_algorithm_functor<N_algorithm_functor, r2vec_t, 3>;
	
public:

	N_algorithm_functor(std::pmr::memory_resource* rsc) noexcept: base_t(rsc) {}

	static bool is_trivially_inside(r2vec_t r)
	{
		return std::abs(r.x * r.y) > 0.5 ||
			(r.x == 0 && (r.y <= 1.0 && r.y >= -1.0)) || 
			(r.y == 0 && (r.x <= 1.0 && r.x >= -1.0));
	}
	static bool is_trivially_outside(r2vec_t r)
	{
		return (r.x >= 1.0 || r.x <= -1.0) ||
			(r.y >= 1.0 || r.y <= -1.0) ||
			(almost_equal(r.x, 0.0) && !almost_equal(r.y, 0.0)) ||
			(almost_equal(r.y, 0.0) && !almost_equal(r.x, 0.0));
	}


private:
	static r2vec_t g0(const r2vec_t& r, const r2vec_t& t)
	{
		return t / r;
	}

	static r2vec_t g1(const r2vec_t& r, const r2vec_t& t)
	{
		return (t + r2vec_t{ 1.0, 1.0 }) / r;
	}

	static r2vec_t gm1(const r2vec_t& r, const r2vec_t& t)
	{
		return (t - r2vec_t{ 1.0, 1.0 }) / r;

	}

public:
	static std::array<dfs_iterator::generator_t, 3> generate_fns_for_tree(const r2vec_t& r)
	{
		return { std::bind_front(g0, r), std::bind_front(g1, r), std::bind_front(gm1, r) };
	}

	static bool stop_iterating_value(const r2vec_t& r, const r2vec_t& t)
	{
		const auto ro = std::max(std::abs(r.x), std::abs(r.y));
		const auto cut_off = 1 / (1.0 - ro);

		return t.abs() > cut_off;
	}

	static r2vec_t root(const r2vec_t& r)
	{
		return { 1.0 / r.x, 1.0 / r.y };
	}
};

}