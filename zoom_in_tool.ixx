export module frc.utils:zoom;
import std;
import :types;
namespace frc
{
/* Helper function to generate the input to a plotting function when trying to
* zoom in around a given point in a given distance
*/
export inline std::tuple<image_metadata_t, std::string>
 generate_domain_and_resolution(std::complex<double> pt, double radius)
{
	return { {resolution_t{ 600, 600 },
		picture_domain_t::make_from_pt_and_radius(pt,radius)},
		std::format("zoom_pt_{}_{}i_radius{}", pt.real(), pt.imag(), radius) };
}


}