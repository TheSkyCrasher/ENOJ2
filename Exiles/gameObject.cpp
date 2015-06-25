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

void GameObject::RenderMesh(Shader* shader, const Matrix4f& mvp)
{
	shader->Update("MVP", mvp * m_transform.GetTransformation());
	shader->Update("Normal", m_transform.GetTransformation());
	m_mesh->Draw();
}

void GameObject::Draw(Shader* shader, Camera* camera)
{
	m_texture->Bind();
	shader->Update("MVP", camera->GetViewProjection() * m_transform.GetTransformation());
	shader->Update("Normal", m_transform.GetTransformation());
	shader->Update("lightDirection", Vector3f(0.0f, -0.5f, 0.5f));
	m_mesh->Draw();
}