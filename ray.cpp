#include "ray.h"

#include <cstdio>
glm::mat3 orthonormalZ(const glm::dvec3& k){
	using glm::dvec3;

	/* dvec3 i = (std::abs(k.x)>EPS ? dvec3(0.0, 1.0, 0.0) : dvec3(1.0, 0.0, 0.0)); */
	dvec3 i = dvec3(k.z-1.1, k.x, k.y);
	i = normalize(cross(i, k));
	/* dvec3 i = rotz * rotx * k; */
	const dvec3 j = cross(k, i);
	/* if(dot(k, i) > 0.1){ */
	/*	   printf("k:%lf %lf %lf\n", k.x, k.y, k.z); */
	/*	   printf("i:%lf %lf %lf\n", i.x, i.y, i.z); */
	/*	   printf("j:%lf %lf %lf\n", j.x, j.y, j.z); */
	/*	   printf("\n"); */
	/* } */
	return glm::mat3(i, j, k);
}

glm::dvec3 normalFromUV(const glm::dvec3& u, const glm::dvec3& v){
	return normalize(cross(u, v));
}
