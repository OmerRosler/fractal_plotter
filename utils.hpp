#pragma once
#include <limits>
#include <cmath>
#include <filesystem>
#include <string>
#include <ranges>

#include "generator.hpp"

template<std::floating_point T>
bool almost_equal(T x, T y, int ulp = 2)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
        // unless the result is subnormal
        || std::fabs(x - y) < std::numeric_limits<T>::min();
}
struct pixel_coordinates
{
    unsigned int x;
    unsigned int y;
};

struct resolution_t
{
    unsigned int width;
    unsigned int height;

    double ratio() const { return double(width) / height; }
};

struct interval_t
{
    double start;
    double end;

    double length() const { return end - start; }
};


// The domain is a rectangle as BMP files are rectangles
struct picture_domain_t
{
    interval_t x;
    interval_t y;

    double area() const { return x.length() * y.length(); }

    bool is_resolution_for_domain(resolution_t res) const
    {
        return almost_equal(res.ratio(), x.length() / y.length());
    }
};

template<std::ranges::range Rng1, std::ranges::range Rng2>
auto cartesian_product(Rng1 rng1,
    Rng2 rng2) ->
    
    std::generator<std::tuple<std::ranges::range_value_t<Rng1>,
    std::ranges::range_value_t<Rng2>>>
{
    for (auto&& x : rng1)
    {
        for (auto&& y : rng2)
        {
            co_yield std::tuple{ std::forward<decltype(x)>(x), 
                std::forward<decltype(y)>(y) };
        }
    }
}