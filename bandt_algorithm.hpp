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

	static dfs_iterator::generator_fns_container_t generate_fns_for_tree(const std::complex<double>& r)
	{
		return { std::bind_front(g0, r),
			std::bind_front(g1, r),
			std::bind_front(gm1, r) };
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

	/*If any translation vector `t`	satisfies this condition then there exists 
	 * an internal point of M within `pixel_size/sqrt(2)` ball around `r`
	 * And this means that we don't know whether the majority of points in this pixel
	 * are outside or inside.
	 * Therefore we decide to draw them black as we want the white part to be the real outside
	 * 
	 * The invariant of the algorithm is that a white pixel is deterministically contained in the
	 * outside of the fractal
	 * 
	 * The explicit bound is Theorem 9.4 of Bandt's paper
	*/
	static bool translation_vector_satsifies_bound_for_outside_point(
		const std::complex<double>& r,
		double pixel_size, 
		const std::complex<double>& t, 
		unsigned int depth)
	{
		const auto ro = std::abs(r);
		const auto ro_prime = ro + pixel_size;
		const auto constant_term = (1 + ro) / (1 - ro);
		const auto dependent_term = std::pow(1.0 / ro, depth);

		const auto bound = 2 * pixel_size * dependent_term / ((1 - ro) * (1 - ro_prime)) + constant_term;

		assert(!almost_equal(bound, 0.0));
		return (std::abs(t) >= bound);
	}
};
}