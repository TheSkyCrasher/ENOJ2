#include "mesh.h"
#include <iostream>

#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

MeshObject::MeshObject(unsigned int indexSize, unsigned int materialIndex)
{
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	m_size = indexSize;
	m_materialIndex = materialIndex;
}

MeshObject::~MeshObject()
{
	if (m_vbo) glDeleteBuffers(1, &m_vbo);
	if (m_ibo) glDeleteBuffers(1, &m_ibo);
}


Mesh::Mesh(Vertex* vertices, unsigned int vertSize, unsigned int* indices, unsigned int indexSize)
{
	m_fileName = "";
	InitMesh(vertices, vertSize, indices, indexSize);
}

Mesh::Mesh(const std::string& fileName)
{
	m_fileName = "Data/models/" + fileName;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(m_fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_FlipUVs);

	if (!scene)
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
			const aiVector3D* pTangent = &(model->mTangents[j]);
			const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][j]) : &aiZeroVector;

			Vertex vert(pPos->x, pPos->y, pPos->z, pTexCoord->x, pTexCoord->y);

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

		InitMesh(&vertices[0], vertices.size(), (unsigned int*)&indices[0], indices.size(), model->mMaterialIndex);
	}

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
		const aiMaterial* pMaterial = scene->mMaterials[i];

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			/*if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_Textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
			}*/
		}
	}
}

Mesh::~Mesh()
{
	for (unsigned int i = 0; i < m_meshObjects.size(); ++i)
	{
		if (m_meshObjects[i]) delete m_meshObjects[i];
	}
}

void Mesh::InitMesh(Vertex* vertices, unsigned int vertSize, unsigned int* indices, unsigned int indexSize, unsigned int materialIndex)
{
	MeshObject* meshObejct = new MeshObject(indexSize, materialIndex);

	CalcNormals(vertices, vertSize, indices, indexSize);

	glBindBuffer(GL_ARRAY_BUFFER, meshObejct->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshObejct->GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(int), indices, GL_STATIC_DRAW);

	m_meshObjects.push_back(meshObejct);
}

void Mesh::Draw() const
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	for (unsigned int i = 0; i < m_meshObjects.size(); ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshObjects[i]->GetVBO());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3f));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)+sizeof(Vector2f)));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)* 2 + sizeof(Vector2f)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshObjects[i]->GetIBO());
		glDrawElements(GL_TRIANGLES, m_meshObjects[i]->GetSize(), GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void Mesh::CalcNormals(Vertex* vertices, unsigned int vertSize, unsigned int* indices, unsigned int indexSize)
{
	for (unsigned int i = 0; i < indexSize; i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		Vector3f v1 = vertices[i1].pos - vertices[i0].pos;
		Vector3f v2 = vertices[i2].pos - vertices[i0].pos;

		Vector3f normal = v1.Cross(v2).Normalized();

		vertices[i0].normal += normal;
		vertices[i1].normal += normal;
		vertices[i2].normal += normal;
	}

	for (unsigned int i = 0; i < vertSize; ++i)
		vertices[i].normal = vertices[i].normal.Normalized();
}