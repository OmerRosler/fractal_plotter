#pragma once
#include <limits>
#include <cmath>
#include <filesystem>
#include <string>
#include <ranges>
#include <concepts>
#include <complex>
#include <functional>
#include <type_traits>
#include <tuple>

#include "generator.hpp"
#include "bitmap_image.hpp"

#include "algorithm_concepts.hpp"

namespace frc
{
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


template<class S, std::size_t...Is, class Tup>
S to_struct_impl(std::index_sequence<Is...>, Tup&& tup) {
    using std::get;
    return { get<Is>(std::forward<Tup>(tup))... };
}
/*
* Utility function to convert tuple to struct
*/
template<class S, class Tup>
S to_struct(Tup&& tup) {
    using T = std::remove_reference_t<Tup>;

    return to_struct_impl<S>(
        std::make_index_sequence < std::tuple_size<T>{} > {},
        std::forward<Tup>(tup)
    );
}


template<std::floating_point T>
constexpr long long log1eps()
{
    return -std::numeric_limits<T>::min_exponent10;
}

/* An arithmetic and comparable type to represent R2 cartesian coordinates
*/

struct r2vec_t
{
    double x;
    double y;

    friend bool operator==(const r2vec_t& v, const r2vec_t& u)
    {
        return almost_equal(v.x, u.x) && almost_equal(v.y, u.y);
    }
    friend bool operator!=(const r2vec_t& v, const r2vec_t& u)
    {
        return !(u == v);
    }

    double norm() const
    {
        return x * x + y * y;
    }
    double abs() const
    {
        return std::sqrt(norm());
    }
    double angle() const
    {
        return std::tan(y / x);
    }
    double dot(const r2vec_t& v) const
    {
        return x * v.x + y * v.y;
    }

    r2vec_t& operator+=(const r2vec_t& v)&
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    r2vec_t& operator-=(const r2vec_t& v)&
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    r2vec_t& operator*=(const r2vec_t& v)&
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }
    r2vec_t& operator/=(const r2vec_t& v)&
    {
        assert(v.x != 0);
        x /= v.x;
        assert(v.y != 0);
        y /= v.y;
        return *this;
    }
    r2vec_t& operator*=(double scalar)&
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    r2vec_t& operator/=(double scalar)&
    {
        assert(scalar != 0);
        x /= scalar;
        y /= scalar;
        return *this;
    }

    r2vec_t operator+(const r2vec_t& v) const
    {
        return { x + v.x, y + v.y };
    }
    r2vec_t operator-(const r2vec_t& v) const
    {
        return { x - v.x, y - v.y };
    }
    r2vec_t operator*(const r2vec_t& v) const
    {
        return { x * v.x, y * v.y };
    }
    r2vec_t operator/(const r2vec_t& v) const
    {
        assert(v.x != 0);
        assert(v.y != 0);
        return { x / v.x, y / v.y };
    }
    r2vec_t operator*(double scalar) const
    {
        return { x * scalar, y * scalar };
    }
    r2vec_t operator/(double scalar) const
    {
        assert(scalar != 0);
        return { x / scalar, y / scalar };
    }
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

    /* We want to find the radius of the minimal circle that contains
    * an entire pixel given the resolution
    */
    long double min_bounding_radius_for_pixel(resolution_t res) const
    {
        return std::max(x.length() / res.width,
            y.length() / res.height) / std::sqrt(2);
    }
};

//This is the metadata used to represent an image
struct image_metadata_t
{
    resolution_t res;
    picture_domain_t dom;

    //Used to convert a point in R2 to the pixel id in the image
    std::pair<unsigned int, unsigned int> pixel_id_from_value(double x, double y) const
    {
        return { std::floor(res.width * (x - dom.x.start) / dom.x.length() - 0.5),
        std::floor(res.height * (y - dom.y.end) / (-dom.y.length()) + 0.5) };
    }

    //Used to convert pixel id to the point in R2 it represents in the image
    std::pair<double, double> pixel_value_from_id(unsigned int px, unsigned int py) const
    {
        return {
            ((dom.x.length() * (px + 0.5)) / res.width + dom.x.start),
            (-dom.y.length() * (py - 0.5) / res.height + dom.y.end)
        };
    }

};


//Note std::move_only_function is C++23 which is too new at the moment
#ifdef __cpp_lib_move_only_function
template<typename T>
using function_holder_t = std::move_only_function<T>;
#else
template<typename T>
using function_holder_t = std::function<T>;
#endif
using color_t = ::bitmap_image::rgb_t;


template<typename F>
concept complex_fractal_algorithm = fractal_algorithm < F, std::complex<double>>;
template<typename T>
using pixel_painter_t = color_t(*)(unsigned int, const T&, unsigned int);

/* A type that declares how much memory it requires will use it,
*  otherwise don't pre-allocate anything
*/
template<typename Alg>
constexpr std::size_t caclulate_pre_allocation_buffer_size(unsigned int max_iterations)
{
    if constexpr (
        requires {
            {Alg::approximate_maximal_dynamic_memory(max_iterations) } ->
                std::convertible_to<std::size_t>; })
    {
        return Alg::approximate_maximal_dynamic_memory(max_iterations);
    }
    else
    {
        return 0;
    }
}
}