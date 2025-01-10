#pragma once

#include <glm/glm.hpp>

#include "shape.h"

class Triangle : public Shape {
public:
	glm::dvec3 m_p1, m_p2, m_p3;

	glm::dvec3 m_norm;
	
	Triangle(){ }
	Triangle(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3,
	         glm::dvec3 col, glm::dvec3 emis);

	Triangle(const Triangle& b) = default;
	Triangle(Triangle&& b) noexcept = default;
	Triangle& operator=(const Triangle& b) = default;
	Triangle& operator=(Triangle&& b) noexcept = default;

	glm::dvec3 normal(const glm::dvec3& point, const glm::dvec3& dir) const;
	double intersect(const Ray& ray) const;
	
	[[nodiscard]] glm::dvec3
	sampleSurface(Ray& ray, double& dist, double zeta1, double zeta2) const;

};
