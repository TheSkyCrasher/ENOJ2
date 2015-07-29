#include "gameObject.h"

#include <iostream>

GameObject::GameObject(Mesh* mesh) :
m_mesh(mesh)
{
	
}

GameObject::~GameObject()
{
	if (m_mesh) delete m_mesh;
}

void GameObject::RenderMesh(Shader* shader)
{
	shader->SetUniform("MP", m_transform.GetTransformation());
	m_mesh->Draw(false);
}

void GameObject::Draw(Shader* shader, Camera* camera)
{
	shader->SetUniform("MVP", camera->GetViewProjection() * m_transform.GetTransformation());
	shader->SetUniform("MP", m_transform.GetTransformation());
	m_mesh->Draw();
}