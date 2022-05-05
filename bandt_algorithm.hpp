#pragma once
#include <complex>
#include <vector>
#include <ranges>

#include "utils.hpp"


struct bandt_algorithm_functor
{

	struct memory_layout_t
	{
		std::vector<std::complex<double>> alg_memory[2];
	};

	bandt_algorithm_functor(memory_layout_t& memory,
		std::size_t capacity = std::pow(2, 24)) :
		t_n(&memory.alg_memory[0]), t_np1(&memory.alg_memory[1])
	{
		t_n->clear();
		t_np1->clear();
		t_n->reserve(capacity);
		t_np1->reserve(capacity);
	}

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
	std::vector<std::complex<double>>* t_n;
	std::vector<std::complex<double>>* t_np1;

	void generate_next_set(std::complex<double> r);

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
};
