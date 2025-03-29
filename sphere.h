#pragma once

#include <glm/glm.hpp>

#include "constants.h"
#include "ray.h"

#include "shape.h"

class Sphere : public Shape {
public:
		double m_r;
		glm::dvec3 m_pos;

		Sphere(){ }

		Sphere(double r,
		       glm::dvec3 pos,
		       glm::dvec3 col,
		       glm::dvec3 emis);

		Sphere(const Sphere& b) = default;
		Sphere(Sphere&& b) noexcept = default;
		Sphere& operator=(const Sphere& b) = default;
		Sphere& operator=(Sphere&& b) noexcept = default;

		glm::dvec3
		normal(const glm::dvec3& point, const glm::dvec3& dir) const;

		double intersect(const Ray& ray) const; 
		
		[[nodiscard]] glm::dvec3
		sampleSurface(Ray& ray, double& dist, double zeta1, double zeta2) const;
};
