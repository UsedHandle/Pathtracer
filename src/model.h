#pragma once

#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "triangle.h"

#define ASSIMP_LOAD_FLAGS ( aiProcess_Triangulate )

struct Mesh {
	unsigned int baseFace;
	glm::mat4 transform;
};

struct Model {
	std::vector<Triangle> m_triangles;

	std::vector<Mesh> m_meshes;	
	Model(std::string filename, glm::vec3 col = glm::vec3(.75f),
		glm::vec3 emis = glm::vec3(0.f), glm::mat4 trans = glm::mat4(1.f));
	
	static inline aiMatrix4x4 glmToAssimp(const glm::mat4& b){
		return aiMatrix4x4(b[0][0], b[1][0], b[2][0], b[3][0],
		                   b[0][1], b[1][1], b[2][1], b[3][1],
		                   b[0][2], b[1][2], b[2][2], b[3][2],
		                   b[0][3], b[1][3], b[2][3], b[3][3]);
	}

	static inline glm::mat4 assimpToglm(const aiMatrix4x4& b) {
		return glm::mat4(b[0][0], b[1][0], b[2][0], b[3][0],
		                 b[0][1], b[1][1], b[2][1], b[3][1],
		                 b[0][2], b[1][2], b[2][2], b[3][2],
		                 b[0][3], b[1][3], b[2][3], b[3][3]);
	}

	void processNode(aiNode* node, aiMatrix4x4 accumTrans = aiMatrix4x4());
	void populateMeshesAndBuffers(const aiScene* scene, glm::vec3 col, glm::vec3 emis);
};
