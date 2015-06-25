#pragma once

#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "shader.h"
#include "camera.h"

class GameObject
{
public:
	GameObject(Mesh* mesh, Texture* texture = new Texture("default.png"));
	virtual ~GameObject();

	void RenderMesh(Shader* shader, const Matrix4f& mvp);
	void Draw(Shader* shader, Camera* camera);

	inline Transform& GetTransform() { return m_transform; }
private:
	Mesh* m_mesh;
	Texture* m_texture;
	Transform m_transform;
};