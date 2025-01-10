#pragma once

#include <glm/glm.hpp>

#include "ray.h"

class Shape {
public:
	glm::dvec3 m_col, m_emis;
	double m_area;

	Shape(){}
	Shape(glm::dvec3 col, glm::dvec3 emis, double area):
		m_col(col), m_emis(emis), m_area(area){};
	Shape(const Shape& b) = default;
	Shape(Shape&& b) noexcept = default;
	Shape& operator=(const Shape& b) = default;
	Shape& operator=(Shape&& b) noexcept = default;
	virtual ~Shape(){};
	virtual glm::dvec3 normal(const glm::dvec3& point, const glm::dvec3& dir) const = 0;
	virtual double intersect(const Ray& ray) const = 0;
	
	[[nodiscard]] virtual glm::dvec3
	sampleSurface(Ray& ray, double& dist, double zeta1, double zeta2) const = 0;
};
