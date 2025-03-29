#pragma once

#include <glm/glm.hpp>

#include "constants.h"
#include "ray.h"

#include "shape.h"

class Sphere : public Shape {
public:
		float m_r;
		glm::vec3 m_pos;

		Sphere() : m_r(0.f), m_pos(glm::vec3(0.f)) { }

		Sphere(float r,
		       glm::vec3 pos,
		       glm::vec3 col,
		       glm::vec3 emis);

		Sphere(const Sphere& b) = default;
		Sphere(Sphere&& b) noexcept = default;
		Sphere& operator=(const Sphere& b) = default;
		Sphere& operator=(Sphere&& b) noexcept = default;

		glm::vec3
		normal(const glm::vec3& point, const glm::vec3& dir) const;

		float intersect(const Ray& ray) const;
		
		[[nodiscard]] glm::vec3
		sampleSurface(Ray& ray, float& dist, float zeta1, float zeta2) const;

		Bound getBound() const;
};
