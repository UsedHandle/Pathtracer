#pragma once

#include <glm/glm.hpp>

#include "constants.h"

struct Ray {
	glm::dvec3 O, D;

	Ray(glm::dvec3 O_in, glm::dvec3 D_in):
		O(O_in), D(D_in) {}
};

glm::mat3 orthonormalZ(const glm::dvec3& z);

glm::dvec3 normalFromUV(const glm::dvec3& u, const glm::dvec3& v);
