#include "MPA_algorithm.hpp"


namespace frc
{
bool will_not_be_in_domain(interval_t idom, double point, unsigned int exp, double param)
{
    bool cond = false;
    //TODO: Calculate powers of the param in advance
    auto pm = std::pow(param, exp);
    if (point < idom.start)
    {
        cond = ((idom.start - pm * point) > ((1 - pm) / (1 - param)));
    }
    else if (point > idom.end)
    {
        cond = ((pm * point - idom.end) > (1 - pm) / (1 - param));
    }
    return cond;
}

bool will_not_be_in_domain(picture_domain_t dom, r2vec_t point, unsigned int exp, r2vec_t param)
{
    return will_not_be_in_domain(dom.x, point.x, exp, param.x) ||
        will_not_be_in_domain(dom.y, point.y, exp, param.y);
}


void draw_entire_attractor(
    const std::string& pic_path,
    image_metadata_t meta,
    unsigned int max_iterations,
    std::vector<ifs_map_data_t> ifs,
    const r2vec_t& param)
{
    assert(meta.dom.is_resolution_for_domain(meta.res));
    bitmap_image fractal_jet(meta.res.width, meta.res.height);

    //set all pixels to white
    fractal_jet.clear(255);

    //TODO: Write a real frame data structure wrapper that works with the algorithm
    std::vector frame(meta.res.height + 1,
        std::vector<int>(meta.res.width + 1));



    std::queue<iterated_point> inside_pixels;
    //Fill the queue with the fixed points of the maps (which are definetly in the attractor)
    for (auto&& [_, fp] : ifs)
    {
        inside_pixels.push({ fp });
    }
    while (!inside_pixels.empty())
    {
        auto [point, num_iterations] = inside_pixels.front();

        auto coords = meta.pixel_id_from_value(point.x, point.y);


        //Mark the point inside
        if (meta.dom.is_in_range(point))
        {
            assert(coords.first >= 0 && coords.first <= meta.res.width, "x");
            assert(coords.second >= 0 && coords.second <= meta.res.height, "y");
            frame[coords.first][coords.second] = 1;
        }

        if (will_not_be_in_domain(meta.dom, point, max_iterations - num_iterations, param))
        {
            inside_pixels.pop();
            continue;
        }

        //Apply all maps to the point and add them to the queue
        for (auto&& [ifs_map, _] : ifs)
        {
            // Early breakout if needed
            if (num_iterations == max_iterations)
            {
                break;
            }
            auto new_point = ifs_map(point);

            inside_pixels.emplace(new_point, num_iterations + 1);
        }

        inside_pixels.pop();
    }

    //TODO: Split this into a utility function of the frame
    for (int i = 0; i < meta.res.height; ++i)
    {
        for (int j = 0; j < meta.res.width; ++j)
        {
            if (frame[i][j] != 0)
            {
                fractal_jet.set_pixel(i, j, 0, 0, 0);
            }
        }
    }
    fractal_jet.save_image(pic_path);
}
}