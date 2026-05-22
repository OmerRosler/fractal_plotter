#include "utils.hpp"
std::pair<std::int64_t, std::int64_t>
    frc::bounded_rational(double r, std::int64_t max_den)
{
    std::int64_t p0 = 0, q0 = 1;
    std::int64_t p1 = 1, q1 = 0;

    while (true)
    {
        auto a = static_cast<std::int64_t>(std::floor(r));

        std::int64_t p2 = a * p1 + p0;
        std::int64_t q2 = a * q1 + q0;

        if (q2 > max_den)
            break;

        p0 = p1; q0 = q1;
        p1 = p2; q1 = q2;

        double frac = r - a;

        if (almost_equal(frac, 0.0))
            break;

        r = 1.0 / frac;
    }

    return { p1, q1 };
}