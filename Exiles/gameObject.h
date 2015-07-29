#pragma once

#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "shader.h"
#include "camera.h"

class GameObject
{
public:
	GameObject(Mesh* mesh);

	virtual ~GameObject();

	void RenderMesh(Shader* shader);
	void Draw(Shader* shader, Camera* camera);

	void SetShiness(unsigned int shiness) { m_shiness = shiness; }
	void SetGloom(unsigned int gloom) { m_gloom = gloom; }

	inline Transform& GetTransform() { return m_transform; }
private:
	unsigned int m_shiness;
	unsigned int m_gloom;

	Mesh* m_mesh;
	Transform m_transform;
};