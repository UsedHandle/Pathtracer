#include "bvh.h"
#include <array>

BVH::BVH(const std::vector<Shape*>& shapeList) {
	shapeBounds.resize(shapeList.size());
	for (size_t i = 0; i < shapeList.size(); ++i) {
		const auto shapeBound = shapeList[i]->getBound();
		shapeBounds[i] =
			ShapeBound(shapeBound, shapeBound.getCentroid(), shapeList[i]);
	}
	root = createNode(0, shapeList.size());
}

std::shared_ptr<BVHNode> BVH::createNode(size_t start, size_t end) {
	// pbrt SAH
	Bound bound;
	for (size_t i = start; i < end; ++i)
		bound = Union(bound, shapeBounds[i].bound);
	size_t nShapes = end - start;
	if (nShapes == 1)
		return std::make_shared<BVHNode>(bound, start);
	
	Bound centroidBound;
	for (size_t i = start; i < end; ++i)
		centroidBound = Union(centroidBound, shapeBounds[i].centroid);
	uint8_t axis = centroidBound.largestAxis();
		
	const bool isFlat = (std::abs(centroidBound.max[axis] - centroidBound.min[axis]) /
		(centroidBound.max[axis] + centroidBound.min[axis])) < EPS;
	if (isFlat) return std::make_shared<BVHNode>(bound, start, nShapes);
	if (nShapes <= 4) {
		size_t mid = (start + end) / 2;
		std::nth_element(&shapeBounds[start], &shapeBounds[mid],
			&shapeBounds[end - 1],
			[axis](const ShapeBound& a, const ShapeBound& b) {
				return a.centroid[axis] < b.centroid[axis];
			});
		return std::make_shared<BVHNode>(bound, createNode(start, mid),
			createNode(mid, end), axis, start, 0);
	}
	constexpr int nBuckets = 12;
	struct BucketInfo {
		int count = 0;
		Bound bound;
	};
	BucketInfo buckets[nBuckets];
	for (size_t i = start; i < end; ++i) {
		int b = std::min(nBuckets - 1,
			(int)(nBuckets * centroidBound.offset(shapeBounds[i].centroid)[axis]));
		++buckets[b].count;
		buckets[b].bound = Union(buckets[b].bound, shapeBounds[i].bound);
	}

	float cost[nBuckets - 1];
	for (int i = 0; i < nBuckets - 1; ++i) {
		Bound b0, b1;
		int count0{}, count1{};
		for (int j = 0; j <= i; ++j) {
			b0 = Union(b0, buckets[j].bound);
			count0 += buckets[j].count;
		}
		for (int j = i + 1; j < nBuckets; ++j) {
			b1 = Union(b1, buckets[j].bound);
			count1 += buckets[j].count;
		}
		
		cost[i] = 0.125f + (count0 * b0.area() + count1 * b1.area()) / bound.area();
	}

	float minCost = cost[0];
	int minCostSplitBucket = 0;
	for (int i = 1; i < nBuckets - 1; ++i) {
		if (cost[i] < minCost) {
			minCost = cost[i];
			minCostSplitBucket = i;
		}
	}
	
	// cost of not splitting
	float leafCost = nShapes;
	if (nShapes <= maxShapesInNode && minCost >= leafCost)
		return std::make_shared<BVHNode>(bound, start, nShapes);
	ShapeBound* midptr = std::partition(&shapeBounds[start], &shapeBounds[end - 1] + 1,
		[=](const ShapeBound& a) {
			int b = std::min(
				(int)(nBuckets * centroidBound.offset(a.centroid)[axis]),
				nBuckets - 1
			);
				return b <= minCostSplitBucket;
		});
	size_t mid = midptr - &shapeBounds[0];
	return std::make_shared<BVHNode>(bound, createNode(start, mid),
		createNode(mid, end), axis, start, 0);
}

bool BVH::findIntersection(
	const Ray& ray,
	float& t,
	const Shape*& shapeptr) const
{
	t = MAX_T;
	int stackIndex = 0;
	std::array<std::shared_ptr<BVHNode>, 64> stack;
	stack.at(stackIndex++) = root;
	while (stackIndex != 0) {
		--stackIndex;
		const std::shared_ptr<BVHNode> node = stack[stackIndex];
		const float node_t = stack.at(stackIndex)->bound.intersect(ray);
		// node hit
		if (node_t < MAX_T ) {
			if (node->nShapes == 0) {
				stack[stackIndex++] = node->children[0];
				stack[stackIndex++] = node->children[1];
				continue;
			}

			// leaf node
			for (size_t i = 0; i < node->nShapes; ++i) {
				const Shape* shape = shapeBounds.at(node->firstShape + i).shape;
				const float shape_t = shape->intersect(ray);
				if (shape_t < t) {
					shapeptr = shape;
					t = shape_t;
				}
			}

		}
	}
	return t < MAX_T;
}

bool BVH::visibility(const Ray& ray, float t) const {
	float temp_t;
	const Shape* shape = nullptr;
	findIntersection(ray, temp_t, shape);
	return t > temp_t + EPS;
}

BVHNode::BVHNode(Bound newBound, size_t firstShape) :
	bound(newBound), firstShape(firstShape), nShapes(1)
{
	children[0] = nullptr; children[1] = nullptr;
}

BVHNode::BVHNode(Bound newBound, size_t firstShape, size_t nShapes) :
	bound(newBound), firstShape(firstShape), nShapes(nShapes)
{
	children[0] = nullptr; children[1] = nullptr;
}

BVHNode::BVHNode(Bound newBound, std::shared_ptr<BVHNode> c0, std::shared_ptr<BVHNode> c1,
	uint8_t axis, size_t firstShape, size_t nShapes) :
	bound(newBound), firstShape(firstShape), nShapes(nShapes), splitAxis(axis)
{
	children[0] = c0; children[1] = c1;
}