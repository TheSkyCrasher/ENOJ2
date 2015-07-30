#include "meshObject.h"

void MeshObject::RenderMesh(Shader* shader)
{
	shader->SetUniform("MP", m_transform.GetTransformation());
	m_mesh->Draw(false);
}

void MeshObject::Draw(Shader* shader, Camera* camera)
{
	shader->SetUniform("MVP", camera->GetViewProjection() * m_transform.GetTransformation());
	shader->SetUniform("MP", m_transform.GetTransformation());
	m_mesh->Draw();
}