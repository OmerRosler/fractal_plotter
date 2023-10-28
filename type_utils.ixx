module;
#include <assert.h>
export module frc.utils:types;

import <version>;
import std;
export import stdgen;

export import bitmap_image;


namespace frc
{
/* General tool to compare floating point types
*/
export template<std::floating_point T>
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
export template<class S, class Tup>
    S to_struct(Tup&& tup) {
    using T = std::remove_reference_t<Tup>;

    return to_struct_impl<S>(
        std::make_index_sequence < std::tuple_size<T>{} > {},
        std::forward<Tup>(tup)
    );
}


export template<std::floating_point T>
    constexpr long long log1eps()
{
    return -std::numeric_limits<T>::min_exponent10;
}

/* An arithmetic and comparable type to represent R2 cartesian coordinates
*/

export struct r2vec_t
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

    friend double distance(const r2vec_t& a, const r2vec_t& b)
    {
        return (a - b).abs();
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
    r2vec_t& operator+=(double scalar)&
    {
        x += scalar;
        y += scalar;
        return *this;
    }
    r2vec_t& operator-=(double scalar)&
    {
        x -= scalar;
        y -= scalar;
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

    r2vec_t operator-() const
    {
        return { -x, -y };
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


    r2vec_t operator+(double scalar) const
    {
        return { x + scalar, y + scalar };
    }
    friend r2vec_t operator+(double scalar, const r2vec_t& self)
    {
        return self + scalar;
    }

    r2vec_t operator-(double scalar) const
    {
        return { x - scalar, y - scalar };
    }
    friend r2vec_t operator-(double scalar, const r2vec_t& self)
    {
        return (-self) + scalar;
    }

    r2vec_t operator*(double scalar) const
    {
        return { x * scalar, y * scalar };
    }
    friend r2vec_t operator*(double scalar, const r2vec_t& self)
    {
        return self * scalar;
    }

    r2vec_t operator/(double scalar) const
    {
        assert(scalar != 0);
        return { x / scalar, y / scalar };
    }
    friend r2vec_t operator/(double scalar, const r2vec_t& self)
    {
        return { scalar / self.x, scalar / self.y };
    }
};

export struct pixel_coordinates_t
{
    unsigned int x;
    unsigned int y;
};

export struct resolution_t
{
    unsigned int width;
    unsigned int height;

    inline double ratio() const { return double(width) / height; }
};

export struct interval_t
{
    double start;
    double end;

    inline double length() const { return end - start; }

    inline bool is_inside(double d) const
    {
        return (d >= start) && (d <= end);
    }
};


// The domain is a rectangle as BMP files are rectangles
export struct picture_domain_t
{
    interval_t x;
    interval_t y;

    inline double area() const { return x.length() * y.length(); }

    inline bool is_resolution_for_domain(resolution_t res) const
    {
        return almost_equal(res.ratio(), x.length() / y.length());
    }
    inline bool is_in_range(const r2vec_t& vec) const
    {
        return x.is_inside(vec.x) && y.is_inside(vec.y);
    }

    /* We want to find the radius of the minimal circle that contains
    * an entire pixel given the resolution
    */
    inline long double min_bounding_radius_for_pixel(resolution_t res) const
    {
        return std::max(x.length() / res.width,
            y.length() / res.height) / std::sqrt(2);
    }
};

//This is the metadata used to represent an image
export struct image_metadata_t
{
    resolution_t res;
    picture_domain_t dom;

    //Used to convert a point in R2 to the pixel id in the image
    std::pair<int, int> pixel_id_from_value(double x, double y) const
    {
        //TODO: Why was I adding 0.5 and 1?
        /*return { std::floor(res.width * (x - dom.x.start) / dom.x.length() - 0.5)+1,
        std::floor(res.height * (y - dom.y.end) / (-dom.y.length()) + 0.5)+1 };*/
        return { std::floor(res.width * (x - dom.x.start) / dom.x.length()),
        std::floor(res.height * (y - dom.y.end) / (-dom.y.length())) };
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
export template<typename T>
    using function_holder_t = std::move_only_function<T>;
#else
//TODO: This case does not work, probably due to `const` incorrectness of std::function
export template<typename T>
    using function_holder_t = std::function<T>;
#endif
export using color_t = ::bitmap_image::rgb_t;

export template<typename T>
    using pixel_painter_t = color_t(*)(unsigned int, const T&, unsigned int);

/* A type that declares how much memory it requires will use it,
*  otherwise don't pre-allocate anything
*/
export template<typename Alg>
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

/* The minimal amount of data the MPA algorithm requires:
* the IFS maps and their fixed points
*/
export struct ifs_map_data_t
{
	function_holder_t<r2vec_t(r2vec_t)> map;
	r2vec_t fixed_point;

    ifs_map_data_t(function_holder_t<r2vec_t(r2vec_t)>&& fn, const r2vec_t& fp):
        map(std::move(fn)), fixed_point(fp)
    {
        auto res = map(fixed_point);
        assert(almost_equal(res.x, fixed_point.x) &&
            almost_equal(res.y, fixed_point.y));
    }
};
}