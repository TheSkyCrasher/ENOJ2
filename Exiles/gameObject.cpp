#include "gameObject.h"

#include <iostream>

GameObject::GameObject(Mesh* mesh, Texture* specularMap) :
m_mesh(mesh),  m_specularMap(specularMap)
{
	
}

GameObject::~GameObject()
{
	if (m_mesh) delete m_mesh;
	if (m_specularMap) delete m_specularMap;
}

void GameObject::RenderMesh(Shader* shader)
{
	shader->SetUniform("MP", m_transform.GetTransformation());
	m_mesh->Draw();
}

void GameObject::Draw(Shader* shader, Camera* camera)
{
	m_specularMap->Bind(3);

	shader->SetUniform("MVP", camera->GetViewProjection() * m_transform.GetTransformation());
	shader->SetUniform("MP", m_transform.GetTransformation());
	m_mesh->Draw();
}