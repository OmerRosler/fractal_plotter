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

#include "utils.hpp"


struct N_algorithm_functor
{

	struct memory_layout_t
	{
		std::vector<r2vec_t> alg_memory[2];
	};

	N_algorithm_functor(memory_layout_t& memory,
		std::size_t capacity = std::pow(2, 28)) :
		t_n(&memory.alg_memory[0]), t_np1(&memory.alg_memory[1])
	{
		t_n->clear();
		t_np1->clear();
		t_n->reserve(capacity);
		t_np1->reserve(capacity);
	}

	unsigned int operator()(r2vec_t r, unsigned int max_iterations);

	static bool is_trivially_inside(r2vec_t r)
	{
		return std::abs(r.x * r.y) > 0.5 ||
			(r.x == 0 && (r.y <= 1.0 && r.y >=1.0)) || 
			(r.y == 0 && (r.x <= 1.0 && r.x >= 1.0));
	}
	static bool is_trivially_outside(r2vec_t r)
	{
		return (r.x > 1.0 || r.x < -1.0 || r.y > 1.0 || r.y < -1.0);
	}

private:
	std::vector<r2vec_t>* t_n;
	std::vector<r2vec_t>* t_np1;

	void generate_next_set(r2vec_t r);


	static r2vec_t g0(r2vec_t r, r2vec_t t)
	{
		return { t.x / r.x , t.y / r.y };
	}

	static r2vec_t g1(r2vec_t r, r2vec_t t)
	{
		return { (t.x + 1.0) / r.x , (t.y + 1.0) / r.y };
	}

	static r2vec_t gm1(r2vec_t r, r2vec_t t)
	{
		return { (t.x - 1.0) / r.x , (t.y - 1.0) / r.y };
	}

};
