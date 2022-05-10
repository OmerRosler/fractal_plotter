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
//#include "recursive_tree_dfs_iterator.hpp"


class N_algorithm_functor
{
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

public:
	/* This type models an output operator that generates a ternary tree by
	* recursive applications of the functions `g0, g1, gm1`
	* 
	* The iteration is DFS, but there is an additional member function that skips a subtree
	* 
	*/
	class dfs_iterator
	{
		r2vec_t r;
		const unsigned max_depth;
		std::vector<r2vec_t> previous_values;
		std::vector<int> previous_ids;

		constexpr static std::array<decltype(g0)*, 3> next_fns = { g0, g1, gm1 };
		using index_t = decltype(previous_ids)::value_type;

		dfs_iterator() : max_depth(0) {}

		void kill_twos();

		// Check for comparing iterators from different containers
		static bool validate(const dfs_iterator& a, const dfs_iterator& b)
		{
			assert(a.max_depth == b.max_depth);
			assert(a.previous_values[0] == b.previous_values[0]);
		}

	public:

		dfs_iterator(const r2vec_t& r,
			const r2vec_t& start_value,
			unsigned int max_depth) :
			r(r), max_depth(max_depth), previous_values{}, previous_ids{}
		{
			previous_values.reserve(max_depth + 1);
			previous_ids.reserve(max_depth);

			this->reset(r, start_value, max_depth);
		}
		dfs_iterator(unsigned int max_depth) : dfs_iterator({}, {}, max_depth) {}

		dfs_iterator(const dfs_iterator&) = default;
		dfs_iterator(dfs_iterator&&) = default;
		~dfs_iterator() = default;

		//special member functions
		dfs_iterator& operator=(const dfs_iterator& other)
		{
			if (&other != this)
			{
				validate(*this, other);
				r = other.r;
				previous_values = other.previous_values;
				previous_ids = other.previous_ids;
			}
			return *this;
		}
		dfs_iterator& operator=(dfs_iterator&& other)
		{
			if (&other != this)
			{
				validate(*this, other);
				r = std::move(other.r);
				previous_values = std::move(other.previous_values);
				previous_ids = std::move(other.previous_ids);
			}
			return *this;
		}
		void reset(const r2vec_t& r,
			const r2vec_t& start_value,
			unsigned int max_depth)
		{
			assert(this->max_depth == max_depth);
			this->r = r;

			previous_values.clear();
			previous_ids.clear();

			previous_values.push_back(start_value);
		}
		inline std::size_t depth() const
		{
			assert(previous_ids.size() == previous_values.size()-1);
			return previous_values.size();
		}

		static dfs_iterator null()
		{
			return dfs_iterator();
		}

		dfs_iterator skip_subtree();

		dfs_iterator advance_dfs();

		//iterator interface
		
		inline r2vec_t& operator*()
		{
			return previous_values.back();
		}
		inline const r2vec_t& operator*() const
		{
			return previous_values.back();
		}

		inline r2vec_t* operator->()
		{
			return &**this;
		}

		inline const r2vec_t* operator->() const
		{
			return &**this;
		}

		dfs_iterator& operator++()
		{
			advance_dfs();
			return *this;
		}

		dfs_iterator operator++(int)
		{
			auto this_copy = *this;
			++ * this;
			return this_copy;
		}

		friend bool operator==(const dfs_iterator& a, const dfs_iterator& b)
		{
			//a is null iterator
			if (a.max_depth == 0)
			{
				//b is null iterator
				if (a.max_depth == b.max_depth)
				{
					return true;
				}
				//b is in beyond-the-end state
				if (b.depth() == 1)
				{
					return true;
				}
				return false;
			}
			//b is a null iterator
			if (b.max_depth == 0)
			{
				//a is in beyond-the-end state
				if (a.depth() == 1)
				{
					return true;
				}
				return false;
			}
			validate(a, b);

			//compare position vectors
			return std::ranges::equal(a.previous_ids, b.previous_ids);
		}

		friend bool operator!=(const dfs_iterator& a, const dfs_iterator& b)
		{
			return !(a == b);
		}

		std::ptrdiff_t operator-(const dfs_iterator& other) const
		{
			assert(other != null());
			validate(*this, other);
			std::ptrdiff_t diff = 0;

			//find common prefix of locations
			auto [id_iter1, id_iter2] = 
				std::ranges::mismatch(previous_ids, other.previous_ids);
			
			//same location
			if (id_iter1 == previous_ids.end())
			{
				return diff;
			}
			assert(*id_iter1 != *id_iter2);
			if (*id_iter1 > *id_iter2)
			{
				auto other_copy = other;
				while (other_copy != *this)
				{
					++other_copy;
					++diff;
				}
				return diff;
			}
			else
			{
				auto this_copy = *this;
				while (this_copy != other)
				{
					++this_copy;
					++diff;
				}
				return -diff;
			}
		}
	};

	//recursive_tree_dfs_iterator<r2vec_t, 3>& new_tree_iterator;
	dfs_iterator& tree_iterator;

public:
	using memory_layout_t = dfs_iterator;

	using gen_t = std::function<r2vec_t(const r2vec_t&)>;

	std::array<gen_t, 3> generate_array(const r2vec_t& r)
	{
		return { std::bind_front(g0, r), std::bind_front(g1, r), std::bind_front(gm1, r) };
	}

	N_algorithm_functor(memory_layout_t& memory) : tree_iterator(memory)/*, new_tree_iterator(generate_array(memory.r))*/ {}

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

};

static_assert(std::output_iterator<N_algorithm_functor::dfs_iterator, r2vec_t>);
