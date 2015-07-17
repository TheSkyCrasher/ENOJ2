#include "gameObject.h"

#include <iostream>

GameObject::GameObject(Mesh* mesh, Texture* diffuse, Texture* normalMap, Texture* specularMap) :
m_mesh(mesh), m_diffuse(diffuse), m_normalMap(normalMap), m_specularMap(specularMap)
{
	
}

GameObject::~GameObject()
{
	if (m_mesh) delete m_mesh;
	if (m_diffuse) delete m_diffuse;
	if (m_normalMap) delete m_normalMap;
}

void GameObject::RenderMesh(Shader* shader)
{
	shader->Update("MP", m_transform.GetTransformation());
	m_mesh->Draw();
}

void GameObject::Draw(Shader* shader, Camera* camera)
{
	m_diffuse->Bind(0);
	m_normalMap->Bind(1);
	m_specularMap->Bind(2);

	shader->Update("MVP", camera->GetViewProjection() * m_transform.GetTransformation());
	shader->Update("MP", m_transform.GetTransformation());
	m_mesh->Draw();
}