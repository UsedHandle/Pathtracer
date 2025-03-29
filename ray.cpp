#include "ray.h"

#include <cstdio>
glm::mat3 orthonormalZ(const glm::vec3& k){
	using glm::vec3;

	vec3 i = vec3(k.z-1.1, k.x, k.y);
	i = normalize(cross(i, k));
	const vec3 j = cross(k, i);
	return glm::mat3(i, j, k);
}

glm::vec3 normalFromUV(const glm::vec3& u, const glm::vec3& v){
	return normalize(cross(u, v));
}
