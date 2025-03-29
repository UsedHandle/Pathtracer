#include "sampler.h"
#include "constants.h"


float Sampler::uniform_dist(){
	constexpr float INV_UINT32_MAX =
		1.f/static_cast<float>(UINT32_MAX);
   
	return 
		static_cast<float>(getNextState()) * INV_UINT32_MAX;
}


glm::vec3 Sampler::uniHemisphere(){
	const float cos_theta = uniform_dist();
	// from 0 to PI sin(theta) = sqrt(1 - cos(theta)^2)
	// when sin(theta) is negative the shortcut will
	// still be positive
	const float sin_theta = std::sqrt(1.f - cos_theta*cos_theta);
	const float phi = 2.f * PI * uniform_dist();

	return glm::vec3(
		sin_theta * std::cos(phi),
		sin_theta * std::sin(phi),
		cos_theta
	);
}


glm::vec3 Sampler::cosHemisphere(){
	// samples disk then projects onto hemisphere
	const float phi = 2.f*PI*uniform_dist();
	const float r2 = uniform_dist();
	// r = sin_theta 
	const float r = std::sqrt(r2);

	return glm::vec3(
		r * std::cos(phi),
		r * std::sin(phi),
		std::sqrt(1.f-r2)
	);
}
