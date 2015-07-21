#include "mesh.h"
#include <iostream>

#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

MeshObject::MeshObject(unsigned int indexSize)
{
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	m_size = indexSize;
}

MeshObject::~MeshObject()
{
	if (m_vbo) glDeleteBuffers(1, &m_vbo);
	if (m_ibo) glDeleteBuffers(1, &m_ibo);
}

Mesh::Mesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals)
{
	InitMesh(vertices, vertSize, indices, indexSize, calcNormals);
}

Mesh::Mesh(const std::string& fileName)
{
	m_fileName = "Data/models/" + fileName;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(m_fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
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

		InitMesh(&vertices[0], vertices.size(), (int*)&indices[0], indices.size(), false);
	}

	m_size = m_meshObjects.size();
}

void Mesh::InitMesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals)
{
	MeshObject* m_meshObject = new MeshObject(indexSize);

	glBindBuffer(GL_ARRAY_BUFFER, *m_meshObject->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_meshObject->GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(int), indices, GL_STATIC_DRAW);

	m_meshObjects.push_back(m_meshObject);
}

Mesh::~Mesh()
{
	for (unsigned int i = 0; i < m_meshObjects.size(); ++i)
	{
		if (m_meshObjects[i]) delete m_meshObjects[i];
	}
}

void Mesh::Draw() const
{
	for (unsigned int i = 0; i < m_size; ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, *m_meshObjects[i]->GetVBO());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3f));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)+sizeof(Vector2f)));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)*2+sizeof(Vector2f)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_meshObjects[i]->GetIBO());
		glDrawElements(GL_TRIANGLES, m_meshObjects[i]->GetSize(), GL_UNSIGNED_INT, 0);
	}
}