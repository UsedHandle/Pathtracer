#pragma once

#include <glm/glm.hpp>

#include "ray.h"

class Shape {
public:
	glm::vec3 m_col, m_emis;
	float m_area;

	Shape(){}
	Shape(glm::vec3 col, glm::vec3 emis, float area):
		m_col(col), m_emis(emis), m_area(area){};
	Shape(const Shape& b) = default;
	Shape(Shape&& b) noexcept = default;
	Shape& operator=(const Shape& b) = default;
	Shape& operator=(Shape&& b) noexcept = default;
	virtual ~Shape(){};
	virtual glm::vec3 normal(const glm::vec3& point, const glm::vec3& dir) const = 0;
	virtual float intersect(const Ray& ray) const = 0;
	
	[[nodiscard]] virtual glm::vec3
	sampleSurface(Ray& ray, float& dist, float zeta1, float zeta2) const = 0;
};
