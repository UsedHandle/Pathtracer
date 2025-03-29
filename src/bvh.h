#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "shape.h"
#include "bounds.h"

struct ShapeBound {
	Bound bound;
	glm::vec3 centroid;
	const Shape* shape;
};

struct BVHNode {
	Bound bound;
	std::shared_ptr<BVHNode> children[2];
	uint8_t splitAxis;
	size_t firstShape, nShapes;

	BVHNode() = default;

	BVHNode(Bound newBound, size_t firstShape);
	BVHNode(Bound newBound, size_t firstShape, size_t nShapes);
	BVHNode(Bound newBound, std::shared_ptr<BVHNode> c0, std::shared_ptr<BVHNode> c1,
		uint8_t axis, size_t firstShape, size_t nShapes);
};

// not cache optimized
class BVH{
	std::vector<ShapeBound> shapeBounds;
	std::shared_ptr<BVHNode> root;

	static constexpr int maxShapesInNode = 6;
	std::shared_ptr<BVHNode> createNode(size_t start, size_t end);
public:
	BVH(const std::vector<Shape*>& shapeList);
	BVH() = default;
	BVH(const BVH& b) = default;
	BVH(BVH&& b) noexcept = default;
	BVH& operator=(const BVH& b) = default;
	BVH& operator=(BVH&& b) noexcept = default;

	bool findIntersection(
		const Ray& ray,
		float& t,
		const Shape*& shapeptr) const;

	bool visibility(const Ray& ray, float t) const;
};