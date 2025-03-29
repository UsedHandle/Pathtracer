#pragma once

#include <glm/glm.hpp>

#include <algorithm>

#include "ray.h"

struct Bound {
	glm::vec3 min, max;
	Bound(glm::vec3 a) : min(a), max(a) {}
	Bound(glm::vec3 a, glm::vec3 b)
		: min(a), max(b) { }

	Bound(const Bound& b) = default;
	Bound(Bound&& b) noexcept = default;
	Bound& operator=(const Bound& b) = default;
	Bound& operator=(Bound&& b) noexcept = default;

	float intersect(const Ray& ray) const;
};

inline Bound Union(const Bound& a, const Bound& b){
	return Bound(glm::vec3(std::min(a.min.x, b.min.x),
	                       std::min(a.min.y, b.min.y),
	                       std::min(a.min.z, b.min.z)),
	             glm::vec3(std::max(a.max.x, b.max.x),
	                       std::max(a.max.y, b.max.y),
	                       std::max(a.max.z, b.max.z)));
}
