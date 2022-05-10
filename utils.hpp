#pragma once
#include <limits>
#include <cmath>
#include <filesystem>
#include <string>
#include <ranges>
#include <concepts>
#include <complex>

#include "generator.hpp"
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
};


template<typename F>
concept complex_fractal_algorithm = fractal_algorithm < F, std::complex<double>>;


template<typename F>
concept r2_fractal_algorithm = fractal_algorithm < F, r2vec_t>;

}