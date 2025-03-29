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

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

struct Model {
	std::vector<Triangle> m_triangles;

	std::vector<Mesh> m_meshes;	
	Model(std::string filename, glm::mat4 trans = glm::mat4(1.0)){
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename.c_str(), ASSIMP_LOAD_FLAGS);
		

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			fprintf(stderr, "Assimp: %s\n", importer.GetErrorString());
			exit(EXIT_FAILURE);
		}
		
		m_meshes.resize(scene->mNumMeshes);
		
		processNode(scene->mRootNode, glmToAssimp(trans));
		populateMeshesAndBuffers(scene);
	}
	
	static inline aiMatrix4x4 glmToAssimp(const glm::mat4& b){
		return aiMatrix4x4(b[0][0], b[1][0], b[2][0], b[3][0],
		                   b[0][1], b[1][1], b[2][1], b[3][1],
		                   b[0][2], b[1][2], b[2][2], b[3][2],
		                   b[0][3], b[1][3], b[2][3], b[3][3]);
	}

	void processNode(aiNode* node, aiMatrix4x4 accumTrans = aiMatrix4x4());
	void populateMeshesAndBuffers(const aiScene* scene);


};
