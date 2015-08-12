#include "mesh.h"
#include <iostream>
#include <cassert>
#include <fstream>

std::map<std::string, ModelUnit*> Model::s_resourceMap;

Mesh::Mesh(const std::string& name) : m_name(name)
{
	//m_collisionShape = new btEmptyShape;
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
}

Mesh::~Mesh()
{
	//if (m_collisionShape) delete m_collisionShape;
	//m_collisionShape = 0;

	if (m_vbo) glDeleteBuffers(1, &m_vbo);
	if (m_ibo) glDeleteBuffers(1, &m_ibo);
}

btConvexHullShape* Mesh::GetConvexHullShape()
{
	btConvexHullShape* convexShape = new btConvexHullShape();
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		convexShape->addPoint(btVector3(vertices[i].GetX(), vertices[i].GetY(), vertices[i].GetZ()));
	}

	btShapeHull* hull = new btShapeHull(convexShape);
	hull->buildHull(convexShape->getMargin());

	btConvexHullShape* result = new btConvexHullShape();
	bool updateLocalAabb = false;

	for (int i = 0; i<hull->numVertices(); ++i)
	{
		result->addPoint(hull->getVertexPointer()[i], updateLocalAabb);
	}
	result->recalcLocalAabb();

	delete convexShape;
	delete hull;
	return result;
}

ModelUnit::ModelUnit() :
m_collisionShape(new btEmptyShape), m_animator(0)
{

}

ModelUnit::~ModelUnit()
{
	for (unsigned int i = 0; i < m_meshes.size(); ++i)
	{
		if (m_meshes[i]) delete m_meshes[i];
	}

	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i]) delete m_textures[i];
	}

	if (m_collisionShape) delete m_collisionShape;
	m_collisionShape = 0;
}

Model::Model(Vertex* vertices, int vertSize, int* indices, int indexSize)
{
	m_fileName = "";
	InitMesh(vertices, vertSize, indices, indexSize, 0, "");
}

Model::Model(const std::string& fileName)
{
	std::string folder("models/" + fileName.substr(0, fileName.find_first_of(".")) + "/");
	m_fileName = "Data/" + folder + fileName;

	std::map<std::string, ModelUnit*>::const_iterator it = s_resourceMap.find(fileName);
	if (it != s_resourceMap.end())
	{
		m_meshData = it->second;
		m_meshData->AddReference();
	}
	else {
		m_meshData = new ModelUnit();

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(m_fileName.c_str(),
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_FlipUVs);

		if (!scene || scene->mNumMeshes == 0)
		{
			std::cout << "Mesh load failed!: " << m_fileName << std::endl;
			assert(0);
		}

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* mesh = scene->mMeshes[i];

			std::vector<Vertex> vertices;
			std::vector<int> indices;

			const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
			for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
			{
				const aiVector3D* pPos = &(mesh->mVertices[j]);
				const aiVector3D* pNormal = &(mesh->mNormals[j]);
				const aiVector3D* pTangent = &(mesh->mTangents[j]);
				const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][j]) : &aiZeroVector;

				Vertex vert(pPos->x, pPos->y, pPos->z, pTexCoord->x, pTexCoord->y, pNormal->x, pNormal->y, pNormal->z);
				vert.tangent = Vector3f(pTangent->x, pTangent->y, pTangent->z);

				vertices.push_back(vert);
			}

			std::vector<aiVertexWeight>* tempWeightsPerVertex = new std::vector<aiVertexWeight>[mesh->mNumVertices];

			for (unsigned int j = 0; j < mesh->mNumBones; ++j) {
				const aiBone * pBone = mesh->mBones[j];

				for (unsigned int b = 0; b < pBone->mNumWeights; b++) {
					tempWeightsPerVertex[pBone->mWeights[b].mVertexId].push_back(aiVertexWeight(j, pBone->mWeights[b].mWeight));
				}
			}

			for (unsigned int j = 0; j < vertices.size(); ++j) {
				vertices[j].boneIds = Vector4i(0, 0, 0, 0);
				vertices[j].weights = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);

				if (mesh->HasBones()) {
					if (tempWeightsPerVertex[j].size() > 4) {
						std::cerr << "The model has invalid bone weights and is not loaded." << std::endl;

						assert(0);
					}

					for (unsigned int k = 0; k < tempWeightsPerVertex[j].size(); ++k) {
						vertices[j].boneIds[k] = tempWeightsPerVertex[j][k].mVertexId;
						vertices[j].weights[k] = (GLfloat)tempWeightsPerVertex[j][k].mWeight;
					}
				}
			}

			if (tempWeightsPerVertex != NULL) {
				delete[] tempWeightsPerVertex;
				tempWeightsPerVertex = NULL;
			}

			for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
			{
				const aiFace& face = mesh->mFaces[j];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			InitMesh(&vertices[0], vertices.size(), (int*)&indices[0], indices.size(), mesh->mMaterialIndex, std::string(mesh->mName.data));
		}

		for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
			const aiMaterial* material = scene->mMaterials[i];

			aiString path;
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					std::string textureName(path.data);
					std::size_t found = textureName.find_last_of("/\\");
					textureName = textureName.substr(found + 1);
					std::cout << "Diff: " << textureName << "\n";

					m_meshData->m_textures.push_back(new Texture(folder + textureName));
				}
			}
			else {
				std::cout << "Diff: default\n";
				m_meshData->m_textures.push_back(new Texture("images/default_diff.png"));
			}

			if (material->GetTextureCount(aiTextureType_HEIGHT) > 0) {
				if (material->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					std::string textureName(path.data);
					std::size_t found = textureName.find_last_of("/\\");
					textureName = textureName.substr(found + 1);
					std::cout << "Norm: " << textureName << "\n";

					m_meshData->m_textures.push_back(new Texture(folder + textureName));
				}
			}
			else if (material->GetTextureCount(aiTextureType_NORMALS) > 0) {
				if (material->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					std::string textureName(path.data);
					std::size_t found = textureName.find_last_of("/\\");
					textureName = textureName.substr(found + 1);
					std::cout << "Norm: " << textureName << "\n";

					m_meshData->m_textures.push_back(new Texture(folder + textureName));
				}
			}
			else {
				std::cout << "Norm: default\n";
				m_meshData->m_textures.push_back(new Texture("images/default_norm.png"));
			}

			if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
				if (material->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					std::string textureName(path.data);
					std::size_t found = textureName.find_last_of("/\\");
					textureName = textureName.substr(found + 1);
					std::cout << "Spec: " << textureName << "\n";

					m_meshData->m_textures.push_back(new Texture(folder + textureName));
				}
			}
			else {
				std::cout << "Spec: default\n";
				m_meshData->m_textures.push_back(new Texture("images/default_spec.png"));
			}

			std::cout << m_fileName << "\n" << material->GetTextureCount(aiTextureType_NONE) << " ";
			std::cout << material->GetTextureCount(aiTextureType_DIFFUSE) << " ";
			std::cout << material->GetTextureCount(aiTextureType_SPECULAR) << " ";
			std::cout << material->GetTextureCount(aiTextureType_AMBIENT) << " ";
			std::cout << material->GetTextureCount(aiTextureType_EMISSIVE) << " ";
			std::cout << material->GetTextureCount(aiTextureType_HEIGHT) << " ";
			std::cout << material->GetTextureCount(aiTextureType_NORMALS) << " ";
			std::cout << material->GetTextureCount(aiTextureType_SHININESS) << " ";
			std::cout << material->GetTextureCount(aiTextureType_OPACITY) << " ";
			std::cout << material->GetTextureCount(aiTextureType_DISPLACEMENT) << " ";
			std::cout << material->GetTextureCount(aiTextureType_LIGHTMAP) << " ";
			std::cout << material->GetTextureCount(aiTextureType_REFLECTION) << " ";
			std::cout << material->GetTextureCount(aiTextureType_UNKNOWN) << "\n -------- \n";
		}

		m_meshData->m_scene = importer.GetOrphanedScene();
		if (scene->HasAnimations()) {
			m_meshData->m_animator = new Animator(m_meshData->m_scene, 0);
		}

		s_resourceMap.insert(std::pair<std::string, ModelUnit*>(fileName, m_meshData));
	}

	m_matrixLocation = glGetUniformLocation(10, "boneMatrices");
}

Model::~Model()
{
	if (m_meshData && m_meshData->RemoveReference())
	{
		if (m_fileName.length() > 0)
			s_resourceMap.erase(m_fileName);

		delete m_meshData;
	}
}

void Model::InitMesh(Vertex* vertices, int vertSize, int* indices, int indexSize, unsigned int materialIndex, const std::string& name)
{
	Mesh* m_mesh = new Mesh(name);
	m_mesh->materialIndex = materialIndex;
	m_mesh->numIndices = indexSize;

	for (int i = 0; i < vertSize; ++i)
	{
		m_mesh->vertices.push_back(vertices[i].pos);
	}

	glBindBuffer(GL_ARRAY_BUFFER, *m_mesh->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_mesh->GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(int), indices, GL_STATIC_DRAW);

	m_meshData->m_meshes.push_back(m_mesh);
}

void Model::Draw(Shader* shader)
{
	if (m_meshData->m_animator != NULL)
		m_meshData->m_animator->UpdateAnimation();

	m_matrixLocation = glGetUniformLocation(shader->GetProgram(), "boneMatrices");
	DrawNode(m_meshData->m_scene->mRootNode);
}

void Model::DrawMesh(unsigned int id) const
{
	glBindBuffer(GL_ARRAY_BUFFER, *m_meshData->m_meshes[id]->GetVBO());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)+sizeof(Vector2f)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)* 2 + sizeof(Vector2f)));
	glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)* 3 + sizeof(Vector2f)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)* 3 + sizeof(Vector2f) + sizeof(Vector4i)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_meshData->m_meshes[id]->GetIBO());
	glDrawElements(GL_TRIANGLES, m_meshData->m_meshes[id]->numIndices, GL_UNSIGNED_INT, 0);
}

void Model::DrawNode(aiNode * pNode) {
	for (unsigned int i = 0; i < pNode->mNumMeshes; ++i) {
		const aiMesh* pCurrentMesh = m_meshData->m_scene->mMeshes[pNode->mMeshes[i]];
		Matrix4f* matrices = new Matrix4f[BONES_PER_MESH];

		if ((pCurrentMesh->HasBones()) && (m_meshData->m_animator != NULL)) {
			const std::vector<aiMatrix4x4>& boneMatrices = m_meshData->m_animator->GetBoneMatrices(pNode, i);

			if (boneMatrices.size() != pCurrentMesh->mNumBones) {
				continue;
			}

			for (unsigned int j = 0; j < pCurrentMesh->mNumBones; j++) {
				if (j < BONES_PER_MESH) {
					matrices[j][0][0] = boneMatrices[j].a1;
					matrices[j][0][1] = boneMatrices[j].b1;
					matrices[j][0][2] = boneMatrices[j].c1;
					matrices[j][0][3] = boneMatrices[j].d1;
					matrices[j][1][0] = boneMatrices[j].a2;
					matrices[j][1][1] = boneMatrices[j].b2;
					matrices[j][1][2] = boneMatrices[j].c2;
					matrices[j][1][3] = boneMatrices[j].d2;
					matrices[j][2][0] = boneMatrices[j].a3;
					matrices[j][2][1] = boneMatrices[j].b3;
					matrices[j][2][2] = boneMatrices[j].c3;
					matrices[j][2][3] = boneMatrices[j].d3;
					matrices[j][3][0] = boneMatrices[j].a4;
					matrices[j][3][1] = boneMatrices[j].b4;
					matrices[j][3][2] = boneMatrices[j].c4;
					matrices[j][3][3] = boneMatrices[j].d4;
				}
			}
		}

		//TODO: put glGetAttribLocation in Init func
		glUniformMatrix4fv(m_matrixLocation, BONES_PER_MESH, GL_FALSE, &matrices[0][0][0]);

		delete matrices;

		const unsigned int materialIndex = m_meshData->m_meshes[pNode->mMeshes[i]]->materialIndex;
		if (materialIndex * 3 < m_meshData->m_textures.size())
		{
			if (m_meshData->m_textures[materialIndex * 3]) {
					m_meshData->m_textures[materialIndex * 3]->Bind(1);
			}

			if (m_meshData->m_textures[materialIndex * 3 + 1]) {
				m_meshData->m_textures[materialIndex * 3 + 1]->Bind(2);
			}

			if (m_meshData->m_textures[materialIndex * 3 + 2]) {
					m_meshData->m_textures[materialIndex * 3 + 2]->Bind(3);
			}
		}

		DrawMesh(pNode->mMeshes[i]);
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; ++i) {
		DrawNode(pNode->mChildren[i]);
	}
}