#include "model.h"

Model::Model(std::string filename, glm::vec3 col,
	glm::vec3 emis, glm::mat4 trans) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename.c_str(), ASSIMP_LOAD_FLAGS);


	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		fprintf(stderr, "Assimp: %s\n", importer.GetErrorString());
		exit(EXIT_FAILURE);
	}

	m_meshes.resize(scene->mNumMeshes);

	processNode(scene->mRootNode, glmToAssimp(trans));
	populateMeshesAndBuffers(scene, col, emis);
}

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

void Model::populateMeshesAndBuffers(const aiScene *scene,
	glm::vec3 col, glm::vec3 emis)
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
			m_triangles[m_meshes[i].baseFace + (std::size_t)facei] = Triangle(p1, p2, p3, col, emis);
		}
	}
}

