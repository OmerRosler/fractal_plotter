#pragma once
#include <complex>
#include <vector>
#include <ranges>

#include "utils.hpp"
#include "recursive_tree_dfs_iterator.hpp"

class bandt_algorithm_functor
{
	using dfs_iterator = recursive_tree_dfs_iterator<std::complex<double>, 3>;

public:
	struct memory_layout_t
	{
	};

	bandt_algorithm_functor(memory_layout_t& memory) noexcept {}

	unsigned int operator()(std::complex<double> r, unsigned int max_iterations);

	static bool is_trivially_inside(std::complex<double> r)
	{
		return std::norm(r) > 0.5;
	}

	static bool is_trivially_outside(std::complex<double> r)
	{
		return std::norm(r) < 0.25;
	}

private:

	static std::complex<double> g0(std::complex<double> r, std::complex<double> t)
	{
		return t / r;
	}

	static std::complex<double> g1(std::complex<double> r, std::complex<double> t)
	{
		return (t + 1.0) / r;
	}

	static std::complex<double> gm1(std::complex<double> r, std::complex<double> t)
	{
		return (t - 1.0) / r;
	}

	std::array<dfs_iterator::generator_t, 3> generate_fns_for_tree(const std::complex<double>& r)
	{
		return { std::bind_front(g0, r), std::bind_front(g1, r), std::bind_front(gm1, r) };
	}
};
