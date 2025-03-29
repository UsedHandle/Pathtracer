#pragma once

#include <initializer_list>
#include <vector>
#include <glm/glm.hpp>

#include "constants.h"
#include "shape.h"
#include "sphere.h"
#include "triangle.h"
#include "ray.h"


struct Scene {
	std::vector<Shape*> objects;
	std::size_t firstLightIndex;

	Scene(){}
	Scene(std::initializer_list<Shape*> objList,
		std::initializer_list<Shape*> lightList);


	Scene(const Scene& b) = default;
	Scene(Scene&& b) noexcept = default;
	Scene& operator=(const Scene& b) = default;
	Scene& operator=(Scene&& b) noexcept = default;

	~Scene();

	bool findIntersection(
			const Ray& ray,
			double& t,
			const Shape*& shapeptr) const;

	bool visibility(const Ray& ray, double t) const;
};
