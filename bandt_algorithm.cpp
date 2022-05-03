#include "bandt_algorithm.hpp"

void bandt_algorithm_functor::generate_next_set(std::complex<double> r)
{
    const auto ro = std::abs(r);
    const auto cut_off = 1 / (1.0 - ro);

    t_np1->clear();

    for (const auto& v : *t_n)
    {
        std::complex<double> candidates[] = { g0(r,v), g1(r,v), gm1(r,v) };
        for (const auto& cand : candidates)
        {
            if (std::abs(cand) <= cut_off)
            {
                t_np1->push_back(cand);
            }
        }
    }
}

unsigned int bandt_algorithm_functor::operator()(std::complex<double> r, 
    unsigned int max_iterations)
{
    t_n->clear();
    t_np1->clear();


    //Trivial parts of M
    if (std::abs(r) < 0.5)
    {
        return 0;
    }
    if (r == 0.0 || std::abs(r) > 1.0 / std::sqrt(2))
    {
        return max_iterations + 1;
    }

    t_n->push_back(1.0 / r);

    for (auto i : std::ranges::views::iota(0u, max_iterations))
    {
        if (t_n->empty())
        {
            return i;
        }
        generate_next_set(r);
        std::swap(t_n, t_np1);
    }
    return max_iterations + 1;
}




std::generator<std::tuple<pixel_coordinates, double, unsigned int>>
M_pixel_map(
    resolution_t res,
    picture_domain_t domain,
    unsigned int max_iterations)
{
    std::vector<std::complex<double>> alg_memory[2];
    auto bandt_algorithm = bandt_algorithm_functor(&alg_memory[0], &alg_memory[1]);

    auto pixel_range = cartesian_product(std::ranges::views::iota(0u, res.height),
        std::ranges::views::iota(0u, res.width));

    for (auto[x,y] : pixel_range)
    {
            std::complex<double> r = 0;
            r.real((domain.x.length() * x) / res.width + domain.x.start);
            r.imag((domain.y.length() * y) / res.height + domain.y.start);

            auto i = bandt_algorithm(r, max_iterations);
            co_yield{ {x,y}, std::abs(r), i };

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