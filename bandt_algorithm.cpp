#include "bandt_algorithm.hpp"


std::vector<std::complex<double>> generate_next_set(std::complex<double> r,
    const std::vector<std::complex<double>>& previous_set)
{
    const auto ro = std::abs(r);
    const auto cut_off = 1 / (1.0 - ro);

    std::vector<std::complex<double>> results = {};
    if (previous_set.size() <= 100)
    {
        results.reserve(std::pow(previous_set.size(), 3));
    }

    for (const auto& v : previous_set)
    {
        std::complex<double> candidates[] = { g0(r,v), g1(r,v), gm1(r,v) };
        for (auto cand : candidates)
        {
            if (std::abs(cand) <= cut_off)
            {
                results.push_back(cand);
            }
        }
    }
    return results;
}

unsigned int bandt_algorithm(std::complex<double> r, unsigned int max_iterations)
{
    //Trivial parts of M
    if (std::abs(r) < 0.5)
    {
        return 0;
    }
    if (r == 0.0 || std::abs(r) > 1.0 / std::sqrt(2))
    {
        return max_iterations + 1;
    }
    
    std::vector<std::complex<double>> prev_level = { 1.0 / r };
    std::vector<std::complex<double>> next_level;
    for (auto i : std::ranges::views::iota(0u, max_iterations))
    {
        if (prev_level.empty())
        {
            return i;
        }
        next_level = generate_next_set(r, prev_level);
        prev_level = std::move(next_level);
    }
    return max_iterations + 1;
}



std::generator<std::tuple<pixel_coordinates, double, unsigned int>>
M_pixel_map(
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations)
{
    for (unsigned int y : std::ranges::views::iota(0u, res.height))
    {
        for (unsigned int x : std::ranges::views::iota(0u, res.width))
        {
            std::complex<double> r = 0;
            r.real(domain.x.length() * x / res.width + domain.x.start);
            r.imag(domain.y.length() * y / res.height + domain.y.start);

            auto i = bandt_algorithm(r, max_iterations);
            co_yield{ {x,y}, std::abs(r), i };

        }
    }
}

void draw_M(
    const std::string& pic_path,
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations)
{
    assert(domain.is_resolution_for_domain(res));
    bitmap_image fractal_jet(res.width, res.height);

    fractal_jet.clear();

    for (auto&& [pixel, magnitude, escape_index] :
        M_pixel_map(res, domain, max_iterations))
    {
        // The intersection does not have an accumulation point
        if (escape_index == max_iterations + 1)
        {
            if (magnitude <= 1.0 / std::sqrt(2))
            {
                fractal_jet.set_pixel(pixel.x, pixel.y, 0, 0, 0); 
            }
            else
            {
                fractal_jet.set_pixel(pixel.x, pixel.y, 100, 100, 100);
            }

            continue;
        }

        //It is outside
        fractal_jet.set_pixel(pixel.x, pixel.y, 255, 255, 255);
    }

    fractal_jet.save_image(pic_path);
}