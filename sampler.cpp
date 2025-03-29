#include "sampler.h"
#include "constants.h"

Sampler& Sampler::operator=(const Sampler& b){
	state=b.state;
	return *this;
}

[[nodiscard]] double Sampler::uniform_dist(){
	constexpr double INV_UINT32_MAX =
		1.0/static_cast<double>(UINT32_MAX);
   
	return 
		static_cast<double>(getNextState()) * INV_UINT32_MAX;
}

[[nodiscard]]
glm::dvec3 Sampler::uniHemisphere(){
	const double cos_theta = uniform_dist();
	// from 0 to PI sin(theta) = sqrt(1 - cos(theta)^2)
	// when sin(theta) is negative the shortcut will
	// still be positive
	const double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);
	const double phi = 2.0 * PI * uniform_dist();

	return glm::dvec3(
		sin_theta * std::cos(phi),
		sin_theta * std::sin(phi),
		cos_theta
	);
}

[[nodiscard]]
glm::dvec3 Sampler::cosHemisphere(){
	// samples disk then projects onto hemisphere
	const double phi = 2.0*PI*uniform_dist();
	const double r2 = uniform_dist();
	// r = sin_theta 
	const double r = std::sqrt(r2);

	return glm::dvec3(
		r * std::cos(phi),
		r * std::sin(phi),
		std::sqrt(1.0-r2)
	);
}
