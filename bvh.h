#pragma once

#include <vector>
#include <shape.h>

#include "bounds.h"

struct BVH{
	std::vector<Shape> shapes;
};

struct BVHNode{
	Bounds bound;
	BVHNode* children[2];
	size_t splitAxis, firstShape, nShapes;
};
