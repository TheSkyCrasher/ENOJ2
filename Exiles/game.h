#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "window.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "input.h"
#include "timer.h"
#include "light.h"
#include "skybox.h"
#include "grass.h"
#include "physics.h"
#include "MeshObject.h"
#include <vector>

class Game
{
public:
	Game() : m_defaultShader("model") {}
	virtual ~Game();

	void Start();

	virtual void Init() = 0;
	virtual void Update() = 0;

	void AddToScene(MeshObject* gameObject) { m_objects.push_back(gameObject); Physics::AddBody(gameObject); }
	void SetCamera(Camera* camera) { m_mainCamera = camera; }
private:
	std::vector<MeshObject*> m_objects;
	Camera* m_mainCamera;
	DirectionalLight m_light;
	SkyBox m_skybox;
	Grass m_grass;

	Shader m_defaultShader;
};