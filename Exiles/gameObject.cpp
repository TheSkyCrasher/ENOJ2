#include "gameObject.h"

#include <iostream>

GameObject::GameObject(Mesh* mesh, Texture* texture) :
	m_mesh(mesh), m_texture(texture)
{
	
}

GameObject::~GameObject()
{
	if (m_mesh) delete m_mesh;
	if (m_texture) delete m_texture;
}

void GameObject::RenderMesh(Shader* shader, Camera* camera)
{
	shader->Bind();
	shader->Update(m_transform, *camera);
	m_mesh->Draw();
}

void GameObject::Draw(Shader* shader, Camera* camera)
{
	shader->Bind();
	m_texture->Bind();
	shader->Update(m_transform, *camera);
	m_mesh->Draw();
}