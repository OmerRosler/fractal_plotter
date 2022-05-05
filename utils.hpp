#pragma once
#include <limits>
#include <cmath>
#include <filesystem>
#include <string>
#include <ranges>
#include <concepts>
#include <complex>

#include "generator.hpp"

/* General tool to compare floating point types
*/
template<std::floating_point T>
bool almost_equal(T x, T y, int ulp = 2)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
        // unless the result is subnormal
        || std::fabs(x - y) < std::numeric_limits<T>::min();
}


struct r2vec_t
{
    double x;
    double y;
};

struct pixel_coordinates_t
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
