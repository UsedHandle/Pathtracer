#pragma once

#include <initializer_list>
#include <vector>
#include <glm/glm.hpp>

#include "constants.h"
#include "shape.h"
#include "ray.h"
#include "bvh.h"

class Scene {
	BVH bvh;
public:
	std::vector<Shape*> objects;
	std::vector<Shape*> lights;

	Scene(const std::initializer_list<Shape*>& objList,
		const std::initializer_list<Shape*>& lightList);


	Scene(const Scene& b) = default;
	Scene(Scene&& b) noexcept = default;
	Scene& operator=(const Scene& b) = default;
	Scene& operator=(Scene&& b) noexcept = default;

	~Scene();

	inline bool findIntersection(
		const Ray& ray,
		float& t,
		const Shape*& shapeptr) const {
		return bvh.findIntersection(ray, t, shapeptr);
	}

	inline bool visibility(const Ray& ray, float t) const {
		return bvh.visibility(ray, t);
	}
};
