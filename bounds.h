#pragma once

#include <glm/glm.hpp>

struct Bound {
	glm::vec3 min, max;
	Bound(glm::vec3 a) : min(a), max(b){}
	Bound(glm::vec3 a, glm::vec3 b)
		: min(glm::vec3(std::min(a.x, b.x),std::min(a.y,b.y),
		                std::min(a.z, b.z))),
		  max(glm::vec3(std::max(a.x, b.x),std::max(a.y,b.y))){}
	

	Bound(const Bound& b) = default;
	Bound(Bound&& b) noexcept = default;
	Bound& operator=(const Bound& b) = default;
	Bound& operator=(Bound&& b) noexcept = default;
}

inline Bound Union(const Bound& a, const Bound& b){
	return Bound(glm::vec3(std::min(a.min.x, b.min.x),
	                       std::min(a.min.y, b.min.y),
	                       std::min(a.min.z, b.min.z)),
	             glm::vec3(std::max(a.max.x, b.max.x),
	                       std::max(a.max.y, b.max.y),
	                       std::max(a.max.z, b.max.z)));
}
