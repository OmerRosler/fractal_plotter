#pragma once
#include <complex>
#include <vector>
#include <ranges>

#include "utils.hpp"
#include "recursive_tree_dfs_iterator.hpp"
#include "dfs_bandt_algorithm.hpp"

namespace frc
{
class bandt_algorithm_functor : 
	public dfs_bandt_algorithm_functor<bandt_algorithm_functor, std::complex<double>, 3>
{
	using base_t = dfs_bandt_algorithm_functor<bandt_algorithm_functor, std::complex<double>, 3>;
public:

	bandt_algorithm_functor(std::pmr::memory_resource* rsc) noexcept : base_t(rsc) {}

	static bool is_trivially_inside(std::complex<double> r)
	{
		return std::norm(r) > 0.5;
	}

	static bool is_trivially_outside(std::complex<double> r)
	{
		return std::norm(r) < 0.25;
	}

private:

	static std::complex<double> g0(const std::complex<double>& r, 
		const std::complex<double>& t)
	{
		return t / r;
	}

	static std::complex<double> g1(const std::complex<double>& r, 
		const std::complex<double>& t)
	{
		return (t + 1.0) / r;
	}

	static std::complex<double> gm1(const std::complex<double>& r, 
		const std::complex<double>& t)
	{
		return (t - 1.0) / r;
	}

public:

	static std::array<dfs_iterator::generator_t, 3> generate_fns_for_tree(const std::complex<double>& r)
	{
		return { std::bind_front(g0, r), std::bind_front(g1, r), std::bind_front(gm1, r) };
	}

	static bool stop_iterating_value(const std::complex<double>& r, const std::complex<double>& t)
	{
		const auto ro = std::abs(r);
		const auto cut_off = 1 / (1.0 - ro);

		return std::abs(t) > cut_off;
	}

	static std::complex<double> root(const std::complex<double>& r)
	{
		return 1.0 / r;
	}
};
}