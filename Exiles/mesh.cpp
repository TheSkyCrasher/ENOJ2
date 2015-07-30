#include "mesh.h"
#include <iostream>

#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::map<std::string, MeshSubObjectWrapper*> Mesh::s_resourceMap;

MeshSubObjectWrapper::~MeshSubObjectWrapper()
{
	for (unsigned int i = 0; i < m_meshObjects.size(); ++i)
	{
		if (m_meshObjects[i]) delete m_meshObjects[i];
	}

	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i]) delete m_textures[i];
	}
}

MeshSubObject::MeshSubObject(unsigned int indexSize)
{
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	m_size = indexSize;
}

MeshSubObject::~MeshSubObject()
{
	if (m_vbo) glDeleteBuffers(1, &m_vbo);
	if (m_ibo) glDeleteBuffers(1, &m_ibo);
}

Mesh::Mesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals)
{
	m_fileName = "";
	InitMesh(vertices, vertSize, indices, indexSize, calcNormals, 0);
}

Mesh::Mesh(const std::string& fileName, bool def)
{
	std::string folder;
	if (!def)
		folder = "models/" + fileName.substr(0, fileName.find_first_of(".")) + "/";
	else
		folder = "models/_def/";

	m_fileName = "Data/" + folder + fileName;


	std::map<std::string, MeshSubObjectWrapper*>::const_iterator it = s_resourceMap.find(fileName);
	if (it != s_resourceMap.end())
	{
		m_mw = it->second;
		m_mw->AddReference();
	}
	else {
		m_mw = new MeshSubObjectWrapper();
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(m_fileName.c_str(),
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_FlipUVs);

		if (!scene || scene->mNumMeshes == 0)
		{
			std::cout << "Mesh load failed!: " << m_fileName << std::endl;
			assert(0 == 0);
		}

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* model = scene->mMeshes[i];

			std::vector<Vertex> vertices;
			std::vector<int> indices;

			const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
			for (unsigned int j = 0; j < model->mNumVertices; ++j)
			{
				const aiVector3D* pPos = &(model->mVertices[j]);
				const aiVector3D* pNormal = &(model->mNormals[j]);
				const aiVector3D* pTangent = &(model->mTangents[j]);
				const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][j]) : &aiZeroVector;

				Vertex vert(pPos->x, pPos->y, pPos->z, pTexCoord->x, pTexCoord->y, pNormal->x, pNormal->y, pNormal->z);
				vert.tangent = Vector3f(pTangent->x, pTangent->y, pTangent->z);

				vertices.push_back(vert);
			}

			for (unsigned int j = 0; j < model->mNumFaces; ++j)
			{
				const aiFace& face = model->mFaces[j];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			InitMesh(&vertices[0], vertices.size(), (int*)&indices[0], indices.size(), false, model->mMaterialIndex);
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

					m_mw->m_textures.push_back(new Texture(folder + textureName));
				}
			}
			else {
				std::cout << "Diff: default\n";
				m_mw->m_textures.push_back(new Texture("images/default_diff.png"));
			}

			if (material->GetTextureCount(aiTextureType_HEIGHT) > 0) {
				if (material->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					std::string textureName(path.data);
					std::size_t found = textureName.find_last_of("/\\");
					textureName = textureName.substr(found + 1);
					std::cout << "Norm: " << textureName << "\n";

					m_mw->m_textures.push_back(new Texture(folder + textureName));
				}
			}
			else if (material->GetTextureCount(aiTextureType_NORMALS) > 0) {
				if (material->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					std::string textureName(path.data);
					std::size_t found = textureName.find_last_of("/\\");
					textureName = textureName.substr(found + 1);
					std::cout << "Norm: " << textureName << "\n";

					m_mw->m_textures.push_back(new Texture(folder + textureName));
				}
			}
			else {
				std::cout << "Norm: default\n";
				m_mw->m_textures.push_back(new Texture("images/default_norm.png"));
			}

			if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
				if (material->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					std::string textureName(path.data);
					std::size_t found = textureName.find_last_of("/\\");
					textureName = textureName.substr(found + 1);
					std::cout << "Spec: " << textureName << "\n";

					m_mw->m_textures.push_back(new Texture(folder + textureName));
				}
			}
			else {
				std::cout << "Spec: default\n";
				m_mw->m_textures.push_back(new Texture("images/default_spec.png"));
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

		m_mw->m_size = m_mw->m_meshObjects.size();
		s_resourceMap.insert(std::pair<std::string, MeshSubObjectWrapper*>(fileName, m_mw));
	}
}

void Mesh::InitMesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals, unsigned int mMaterialIndex)
{
	MeshSubObject* m_meshObject = new MeshSubObject(indexSize);
	m_meshObject->m_materialIndex = mMaterialIndex;

	glBindBuffer(GL_ARRAY_BUFFER, *m_meshObject->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_meshObject->GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(int), indices, GL_STATIC_DRAW);

	m_mw->m_meshObjects.push_back(m_meshObject);
}

Mesh::~Mesh()
{
	if (m_mw && m_mw->RemoveReference())
	{
		if (m_fileName.length() > 0)
			s_resourceMap.erase(m_fileName);

		delete m_mw;
	}
}

void Mesh::Draw(bool drawTextures) const
{
	for (unsigned int i = 0; i < m_mw->m_size; ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, *m_mw->m_meshObjects[i]->GetVBO());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3f));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)+sizeof(Vector2f)));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)*2+sizeof(Vector2f)));

		if (drawTextures)
		{
			const unsigned int materialIndex = m_mw->m_meshObjects[i]->m_materialIndex;

			if (materialIndex * 3 < m_mw->m_textures.size() && m_mw->m_textures[materialIndex * 3]) {
				m_mw->m_textures[materialIndex * 3]->Bind(1);
			}

			if (materialIndex * 3 < m_mw->m_textures.size() && m_mw->m_textures[materialIndex * 3 + 1]) {
				m_mw->m_textures[materialIndex * 3 + 1]->Bind(2);
			}

			if (materialIndex * 3 < m_mw->m_textures.size() && m_mw->m_textures[materialIndex * 3 + 2]) {
				m_mw->m_textures[materialIndex * 3 + 2]->Bind(3);
			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_mw->m_meshObjects[i]->GetIBO());
		glDrawElements(GL_TRIANGLES, m_mw->m_meshObjects[i]->GetSize(), GL_UNSIGNED_INT, 0);
	}
}