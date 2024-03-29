export module frc.algorithms:N_attractor_based_on_MPA;

export import :attractor_MPA;

import bitmap_image;

namespace frc
{

export struct N_attractor_algorithm : MPA_algorithm_base_t
{

    r2vec_t param;
    N_attractor_algorithm(r2vec_t p) noexcept :
        MPA_algorithm_base_t(N_ifs_metadata(p)),
        param(p)
    {}
    /*
    * This is a sufficient condition for the point to not be reachable to the given domain
    * after `n` steps.
    *
    * The condition is a coordinate-wise check and is based on the triangle inequality:
    *
    * d(w_n(x),B)>=d(f^n(x),B)-sup d(w_n,f^n)>d(f^n(x),B)-(1+T+T^2+...+T^(n-1))
    *
    * where B is the rectangle domain
    *
    * If the RHS is larger then zero then the point would be outside B
    *
    * We check this for both coordinates while noting that if `x` was in range, then
    * the distance would only be the `y` coordinate distance, otherwise we use both conditions
    *
    * The condition reduces to the formula in code
    * Note there is transitivity for `exp` (i.e. if the condition is satisfied for `m` it
    * is also satisfied for `m-1`)
    *
    */
    inline bool will_not_be_in_domain(picture_domain_t& dom, r2vec_t point, std::size_t exp) const
    {
        return will_not_be_in_interval(dom.x, point.x, exp, param.x) ||
            will_not_be_in_interval(dom.y, point.y, exp, param.y);
    }


private:
    /*
    * Calculation used for each coordinate
    */
    static inline bool will_not_be_in_interval(
        interval_t idom,
        double point,
        std::size_t exp,
        double param)
    {
        bool cond = false;
        //TODO: Calculate powers of the param in advance?
        auto pm = std::pow(param, exp);
        if (point < idom.start)
        {
            cond = ((idom.start - pm * point) > ((1 - pm) / (1 - param)));
        }
        else if (point > idom.end)
        {
            cond = (std::abs(pm * point - idom.end) > ((1 - pm) / (1 - param)));
        }
        return cond;
    }

    //Used to initialize base in the constructor
    static std::vector<ifs_map_data_t> N_ifs_metadata(r2vec_t param);
};

}