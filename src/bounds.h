#pragma once

#include <glm/glm.hpp>

#include <algorithm>

#include "ray.h"

struct Bound {
	glm::vec3 min, max;

	Bound() :
		min(glm::vec3(std::numeric_limits<float>::max())),
		max(glm::vec3(std::numeric_limits<float>::lowest())) {}
	Bound(glm::vec3 a) : min(a), max(a) {}
	Bound(glm::vec3 a, glm::vec3 b) : min(a), max(b) {}

	Bound(const Bound& b) = default;
	Bound(Bound&& b) noexcept = default;
	Bound& operator=(const Bound& b) = default;
	Bound& operator=(Bound&& b) noexcept = default;

	float intersect(const Ray& ray) const;

	inline glm::vec3 getCentroid() const {
		return (max + min) * 0.5f;
	}

	inline uint8_t largestAxis() const {
		const glm::vec3 diag = max - min;
		if (diag.x > diag.y && diag.x > diag.z) return 0;
		else if (diag.y > diag.z) return 1;
		return 2;
	}

	inline glm::vec3 offset(const glm::vec3& p) const {
		glm::vec3 vec = p - min;
		vec.x /= max.x - min.x;
		vec.y /= max.y - min.y;
		vec.z /= max.z - min.z;
		return vec;
	}

	inline float area() const {
		const glm::vec3 diag = max - min;
		return 2.f * (diag.x * diag.y + diag.y * diag.z + diag.z * diag.x);
	}
};

inline Bound Union(const Bound& a, const Bound& b){
	return Bound(glm::vec3(std::min(a.min.x, b.min.x),
	                       std::min(a.min.y, b.min.y),
	                       std::min(a.min.z, b.min.z)),
	             glm::vec3(std::max(a.max.x, b.max.x),
	                       std::max(a.max.y, b.max.y),
	                       std::max(a.max.z, b.max.z)));
}
