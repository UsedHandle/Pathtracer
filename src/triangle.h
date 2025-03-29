#pragma once

#include <glm/glm.hpp>

#include "shape.h"

class Triangle : public Shape {
public:
	glm::vec3 m_p1, m_p2, m_p3;

	glm::vec3 m_norm;
	
	Triangle() : m_p1(glm::vec3(0.f)), m_p2(glm::vec3(0.f)),
		m_p3(glm::vec3(0.f)), m_norm(glm::vec3(0.f)){ }
	Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
	         glm::vec3 col, glm::vec3 emis);

	Triangle(const Triangle& b) = default;
	Triangle(Triangle&& b) noexcept = default;
	Triangle& operator=(const Triangle& b) = default;
	Triangle& operator=(Triangle&& b) noexcept = default;

	glm::vec3 normal(const glm::vec3& point, const glm::vec3& dir) const;
	float intersect(const Ray& ray) const;
	
	[[nodiscard]] glm::vec3
	sampleSurface(Ray& ray, float& dist, float zeta1, float zeta2) const;

	Bound getBound() const;
};
