#include "model.h"

void Model::processNode(aiNode *node, aiMatrix4x4 accumTrans){
	accumTrans = node->mTransformation*accumTrans;
	for(unsigned int i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], accumTrans);
	
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		std::memcpy(&m_meshes[node->mMeshes[i]].transform[0][0],
		       &accumTrans.Transpose(),
		       sizeof(float)*16);
	}
}

void Model::populateMeshesAndBuffers(const aiScene *scene)
{
	// copy data into mesh array and collect info
	unsigned int totalNumFaces  = 0;
	for(unsigned int i = 0; i < scene->mNumMeshes; i++){
		m_meshes[i].baseFace  = totalNumFaces;
		totalNumFaces += scene->mMeshes[i]->mNumFaces;
	}
	m_triangles.resize(totalNumFaces);
	for(unsigned int i = 0; i < scene->mNumMeshes; i++){
		for(unsigned int facei = 0; facei < scene->mMeshes[i]->mNumFaces; ++facei){
			const auto& ind = scene->mMeshes[i]->mFaces[facei].mIndices;
			const auto& aip1 = scene->mMeshes[i]->mVertices[ind[0]];
			const auto& aip2 = scene->mMeshes[i]->mVertices[ind[1]];
			const auto& aip3 = scene->mMeshes[i]->mVertices[ind[2]];
			const auto& trans = m_meshes[i].transform;
			glm::vec4 objp1(aip1.x,aip1.y,aip1.z,1.); 
			glm::vec4 objp2(aip2.x,aip2.y,aip2.z,1.); 
			glm::vec4 objp3(aip3.x,aip3.y,aip3.z,1.);
			objp1 = trans*objp1; objp2 = trans*objp2; objp3 = trans*objp3;

			const glm::vec3 p1(objp1);
			const glm::vec3 p2(objp2);
			const glm::vec3 p3(objp3);
			const glm::vec3 col(.75, .75, .75);
			const glm::vec3 emis(0.,0.,0.);
			m_triangles[m_meshes[i].baseFace + facei] = Triangle(p1, p2, p3, col, emis);
		}
	}
	/*	for(unsigned int facei = 0; facei < scene->mMeshes[i]->mNumFaces; facei++){*/
	/*		const aiFace& face = scene->mMeshes[i]->mFaces[facei];*/
	/*		memcpy(&indices[(facei*3) + meshes[i].baseIndex],*/
	/*					 face.mIndices,*/
	/*					 sizeof(float)*3);*/
	/*	}
	*/
	/*// indices array is made because it is*/
	/*// not possible to copy directly from*/
	/*// scene->mMeshes[i]->mFaces as it*/
	/*// contains a pointer to a few indices */
	/*// (likely 3) and number of indices*/
	/*std::unique_ptr<unsigned int[]> indices =*/
	/*	std::make_unique<unsigned int[]>(totalNumIndices);*/
	/**/
	/**/
	/*glGenVertexArrays(1, &VAO);*/
	/*glGenBuffers(3, VBOs);*/
	/*glGenBuffers(1, &EBO);*/
	/*glBindVertexArray(VAO);*/
	/**/
	/*// create positions buffer*/
	/*glBindBuffer(GL_ARRAY_BUFFER, VBOs[POSITION_LOC]);*/
	/*glBufferData(GL_ARRAY_BUFFER, totalNumVertices*3*sizeof(float),*/
	/*		nullptr, GL_STATIC_DRAW);*/
	/**/
	/*glEnableVertexAttribArray(POSITION_LOC);*/
	/*glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE,*/
	/*		sizeof(float)*3, nullptr);*/
	/**/
	/**/
	/*// create normals buffer*/
	/*glBindBuffer(GL_ARRAY_BUFFER, VBOs[NORM_LOC]);*/
	/*glBufferData(GL_ARRAY_BUFFER, totalNumVertices*3*sizeof(float),*/
	/*		nullptr, GL_STATIC_DRAW);*/
	/**/
	/*glEnableVertexAttribArray(NORM_LOC);*/
	/*glVertexAttribPointer(NORM_LOC, 3, GL_FLOAT, GL_FALSE,*/
	/*		sizeof(float)*3, nullptr);*/
	/**/
	/**/
	/**/
	/*glBindBuffer(GL_ARRAY_BUFFER, VBOs[TEXCOORD_LOC]);*/
	/*glBufferData(GL_ARRAY_BUFFER, totalNumVertices*3*sizeof(float),*/
	/*		nullptr, GL_STATIC_DRAW);*/
	/**/
	/*glEnableVertexAttribArray(TEXCOORD_LOC);*/
	/*glVertexAttribPointer(TEXCOORD_LOC, 2, GL_FLOAT, GL_FALSE,*/
	/*		sizeof(float)*2, nullptr);*/
	/**/
	/*// copy data into buffers*/
	/*for(unsigned int i = 0; i < scene->mNumMeshes; i++){*/
	/*	// glBufferSubData(target, offsetInBytes, sizeOfData, data)*/
	/*	glBindBuffer(GL_ARRAY_BUFFER, VBOs[POSITION_LOC]);*/
	/*	glBufferSubData(GL_ARRAY_BUFFER,*/
	/*		meshes[i].baseVertex*3*sizeof(float),*/
	/*		scene->mMeshes[i]->mNumVertices*3*sizeof(float),*/
	/*		scene->mMeshes[i]->mVertices);*/
	/**/
	/**/
	/*	glBindBuffer(GL_ARRAY_BUFFER, VBOs[NORM_LOC]);*/
	/*	glBufferSubData(GL_ARRAY_BUFFER,*/
	/*			meshes[i].baseVertex*3*sizeof(float),*/
	/*			scene->mMeshes[i]->mNumVertices*3*sizeof(float),*/
	/*			scene->mMeshes[i]->mNormals);*/
	/**/
	/*	// copy texture coords*/
	/*	// scene->mMeshes[i]->mTextureCoords[0] is 3D vector*/
	/*	for(unsigned int verti = 0; verti < scene->mMeshes[i]->mNumVertices; verti++){*/
	/*		if(scene->mMeshes[i]->mTextureCoords[0] != nullptr){*/
	/*			memcpy(&uvs[(verti+meshes[i].baseVertex)*2],*/
	/*						 &scene->mMeshes[i]->mTextureCoords[0][verti],*/
	/*						 2*sizeof(float));*/
	/*		} else {*/
	/*			uvs[(verti+meshes[i].baseVertex)] = 0.0f;*/
	/*			uvs[(verti+meshes[i].baseVertex)] = 0.0f;*/
	/*		}*/
	/*	}*/
	/**/
	/*	// copy indices*/
	/*	for(unsigned int facei = 0; facei < scene->mMeshes[i]->mNumFaces; facei++){*/
	/*		const aiFace& face = scene->mMeshes[i]->mFaces[facei];*/
	/*		memcpy(&indices[(facei*3) + meshes[i].baseIndex],*/
	/*					 face.mIndices,*/
	/*					 sizeof(float)*3);*/
	/*	}*/
	/*}*/
	/**/
	/*glBindBuffer(GL_ARRAY_BUFFER, VBOs[TEXCOORD_LOC]);*/
	/*glBufferData(GL_ARRAY_BUFFER, totalNumVertices*2*sizeof(float),*/
	/*		&uvs[0], GL_STATIC_DRAW);*/
	/**/
	/**/
	/**/
	/**/
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);*/
	/*glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalNumIndices*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);*/
	/*glBindVertexArray(0);*/
}

