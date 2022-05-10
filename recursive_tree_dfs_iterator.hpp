#pragma once

#include <ranges>
#include <vector>
#include <array>
#include <functional>
#include <cassert>

/* This type models an output operator that generates a ternary tree by
	* recursive applications of an array of functions
	*
	* The iteration is DFS, but there is an additional member function that skips a subtree
	*
	*/
template<typename T, 
	std::size_t N>
class recursive_tree_dfs_iterator
{
	const unsigned max_depth;
	std::vector<T> previous_values;
	std::vector<int> previous_ids;
	using value_generators_t = std::array<std::function<T(const T&)>, N>;
	const value_generators_t new_value_generators;

	recursive_tree_dfs_iterator(value_generators_t gens) : max_depth(0), 
		new_value_generators(gens) {}

	void traverse_up_until_not_last_child()
	{
		//find next "avilable" index
		while ((depth() != 1) && (previous_ids.back() == N -1))
		{
			previous_ids.pop_back();
			previous_values.pop_back();
		}
	}

	// Check for comparing iterators from different containers
	static bool validate(const recursive_tree_dfs_iterator& a, const recursive_tree_dfs_iterator& b)
	{
		assert(a.max_depth == b.max_depth);
		assert(a.previous_values[0] == b.previous_values[0]);
		assert(std::ranges::equal(a.new_value_generators, b.new_value_generators));
	}

	recursive_tree_dfs_iterator() noexcept: max_depth(0), new_value_generators{} 
	{
		static_assert(std::output_iterator<recursive_tree_dfs_iterator, T>);
	}

public:

	using generator_t = typename value_generators_t::value_type;

	recursive_tree_dfs_iterator(value_generators_t gens,
		const T& start_value,
		unsigned int max_depth) :
		max_depth(max_depth), 
		previous_values{}, 
		previous_ids{},
		new_value_generators(gens)
	{
		previous_values.reserve(max_depth + 1);
		previous_ids.reserve(max_depth);

		this->reset(start_value, max_depth);
	}
	recursive_tree_dfs_iterator(value_generators_t gens, 
		unsigned int max_depth) : recursive_tree_dfs_iterator(gens, {}, {}, max_depth) {}

	recursive_tree_dfs_iterator(const recursive_tree_dfs_iterator&) = default;
	recursive_tree_dfs_iterator(recursive_tree_dfs_iterator&&) = default;
	~recursive_tree_dfs_iterator() = default;

	//special member functions
	recursive_tree_dfs_iterator& operator=(const recursive_tree_dfs_iterator& other) noexcept
	{
		if (&other != this)
		{
			validate(*this, other);
			previous_values = other.previous_values;
			previous_ids = other.previous_ids;
		}
		return *this;
	}
	recursive_tree_dfs_iterator& operator=(recursive_tree_dfs_iterator&& other) noexcept
	{
		if (&other != this)
		{
			validate(*this, other);
			previous_values = std::move(other.previous_values);
			previous_ids = std::move(other.previous_ids);
		}
		return *this;
	}
	void reset(const T& start_value,
		unsigned int max_depth) noexcept
	{
		assert(this->max_depth == max_depth);

		previous_values.clear();
		previous_ids.clear();

		previous_values.push_back(start_value);
	}
	inline std::size_t depth() const
	{
		assert(previous_ids.size() == previous_values.size() - 1);
		return previous_values.size();
	}

	static recursive_tree_dfs_iterator null()
	{
		return recursive_tree_dfs_iterator();
	}

	recursive_tree_dfs_iterator skip_subtree()
	{
		//go back up the tree until there is an avilable borther node
		traverse_up_until_not_last_child();
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
		const T last_value = previous_values.back();
		previous_values.emplace_back(new_value_generators[previous_ids.back()](last_value));

		return *this;
	}

	recursive_tree_dfs_iterator advance_dfs()
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
			const T last_value = previous_values.back();
			previous_values.emplace_back(new_value_generators[0](last_value));
		}
		return *this;
	}

	//iterator interface

	inline T& operator*()
	{
		return previous_values.back();
	}
	inline const T& operator*() const
	{
		return previous_values.back();
	}

	inline T* operator->()
	{
		return &**this;
	}

	inline const T* operator->() const
	{
		return &**this;
	}

	recursive_tree_dfs_iterator& operator++()
	{
		advance_dfs();
		return *this;
	}

	recursive_tree_dfs_iterator operator++(int)
	{
		auto this_copy = *this;
		++* this;
		return this_copy;
	}

	friend bool operator==(const recursive_tree_dfs_iterator& a, 
		const recursive_tree_dfs_iterator& b)
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

	friend bool operator!=(const recursive_tree_dfs_iterator& a, 
		const recursive_tree_dfs_iterator& b)
	{
		return !(a == b);
	}

	std::ptrdiff_t operator-(const recursive_tree_dfs_iterator& other) const
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
		//increment `other` until reaching `*this`
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
		//increment `*this` until reaching `other`
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