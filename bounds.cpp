#include "constants.h"
#include "bounds.h"

float Bound::intersect(const Ray& ray) const {
	// pbrt
	float t0 = MIN_T, t1 = MAX_T;
	for (int i = 0; i < 3; ++i) {
		float inv_ray = 1. / ray.D[i];
		// still works if inv_ray == 0
		float farSide = (max[i] - ray.O[i]) * inv_ray;
		float nearSide = (min[i] - ray.O[i]) * inv_ray;
		if (farSide < nearSide) std::swap(farSide, nearSide);

		// if nearSide of farSide are nan then t remains unchanged
		t0 = nearSide > t0 ? nearSide : t0;
		t1 = farSide < t1 ? farSide : t1;
		// ray reenters slab
		if (t0 > t1) return MAX_T;
	}
	return (t0) > MIN_T ? t0 : MIN_T;
}
