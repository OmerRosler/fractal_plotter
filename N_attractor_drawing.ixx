export module frc.drawing:N_attractor;
import :base;
export import frc.algorithms;
namespace frc
{

export void plot_partial_N_attractor(frc::r2vec_t param,
    const std::string& pic_path,
    resolution_t res,
    unsigned int max_iterations)
{

    auto N_ifs_abs_bound = [](double x)
        {
            return x / (1.0 - x);
        };
    auto x_bound = N_ifs_abs_bound(param.x);
    auto y_bound = N_ifs_abs_bound(param.y);
    auto bound = std::max(x_bound, y_bound);

    //This is some subset of the attractor to zoom in to
    image_metadata_t meta = { res,
            frc::picture_domain_t{.x{-bound / 10, bound / 10}, .y{-bound / 10,bound / 10} }
    };

    N_attractor_algorithm algo{ param };

    //TODO: Use `std::mdspan` instead of passing the vector
    std::vector frame(meta.res.height + 1,
        std::vector<int>(meta.res.width + 1));

    //fill the frame data with the algorithm result
    MPA_attractor_output_to_frame(meta, max_iterations, algo, frame);

    //plot it
    bitmap_image fractal_jet(meta.res.width, meta.res.height);

    //set all pixels to white
    fractal_jet.clear(255);

    //Fill picture from frame
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
    //save the image
    fractal_jet.save_image(pic_path);


}

};