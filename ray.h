#pragma once

#include <glm/glm.hpp>

#include "constants.h"

struct Ray {
	glm::vec3 O, D;

	Ray(glm::vec3 O_in, glm::vec3 D_in):
		O(O_in), D(D_in) {}
};

glm::mat3 orthonormalZ(const glm::vec3& z);

glm::vec3 normalFromUV(const glm::vec3& u, const glm::vec3& v);
