#include "mesh.h"
#include <iostream>

#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

MeshObject::~MeshObject()
{
	if (m_vbo) glDeleteBuffers(5, m_vbo);
	if (m_vao) glDeleteVertexArrays(1, &m_vao);
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

		MeshObject* meshObejct = new MeshObject;

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
		for (unsigned int j = 0; j < model->mNumVertices; ++j)
		{
			const aiVector3D* pPos = &(model->mVertices[j]);
			const aiVector3D* pNormal = &(model->mNormals[j]);
			const aiVector3D* pTangent = &(model->mTangents[j]);
			const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][j]) : &aiZeroVector;

			meshObejct->positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
			meshObejct->texCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
			meshObejct->normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
			meshObejct->tangents.push_back(Vector3f(pTangent->x, pTangent->y, pTangent->z));
		}

		for (unsigned int j = 0; j < model->mNumFaces; ++j)
		{
			const aiFace& face = model->mFaces[j];
			assert(face.mNumIndices == 3);
			meshObejct->indices.push_back(face.mIndices[0]);
			meshObejct->indices.push_back(face.mIndices[1]);
			meshObejct->indices.push_back(face.mIndices[2]);
		}

		std::cout << scene->mMaterials[0]->GetTextureCount(aiTextureType_DIFFUSE) << "\n";
		m_meshObjects.push_back(meshObejct);
		Init();
	}
}

void Mesh::Init()
{
	m_meshObjects.back()->m_numIndices = m_meshObjects.back()->indices.size();

	glGenVertexArrays(1, &m_meshObjects.back()->m_vao);
	glBindVertexArray(m_meshObjects.back()->m_vao);

	glGenBuffers(5, m_meshObjects.back()->m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_meshObjects.back()->m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_meshObjects.back()->positions[0]) * m_meshObjects.back()->positions.size(), &m_meshObjects.back()->positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_meshObjects.back()->m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_meshObjects.back()->texCoords[0]) * m_meshObjects.back()->texCoords.size(), &m_meshObjects.back()->texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_meshObjects.back()->m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_meshObjects.back()->normals[0]) * m_meshObjects.back()->normals.size(), &m_meshObjects.back()->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_meshObjects.back()->m_vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_meshObjects.back()->tangents[0]) * m_meshObjects.back()->tangents.size(), &m_meshObjects.back()->tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshObjects.back()->m_vbo[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_meshObjects.back()->indices[0]) * m_meshObjects.back()->indices.size(), &m_meshObjects.back()->indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
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
	for (unsigned int i = 0; i < m_meshObjects.size(); ++i)
	{
		glBindVertexArray(m_meshObjects[i]->m_vao);

		glDrawElementsBaseVertex(GL_TRIANGLES, m_meshObjects[i]->m_numIndices, GL_UNSIGNED_INT, 0, 0);

		glBindVertexArray(0);
	}
}