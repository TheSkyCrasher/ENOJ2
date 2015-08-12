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
#include "terrain.h"
#include <vector>

class Game
{
public:
	Game() : m_defaultShader("model") {}
	virtual ~Game();

	void Start();

	virtual void Init() = 0;
	virtual void Update() = 0;

	Camera* GetCamera() { return m_mainCamera; }

	void AddToScene(MeshObject* gameObject) { m_objects.push_back(gameObject); Physics::AddBody(gameObject); m_renderObjects++; }
	void AddToRenderObjects(MeshObject* gameObject) { m_objects.push_back(gameObject); }
	void SetCamera(Camera* camera) { m_mainCamera = camera; }
private:
	std::vector<MeshObject*> m_objects;
	Camera* m_mainCamera;
	DirectionalLight m_light;
	SkyBox m_skybox;
	Grass m_grass;

	unsigned int m_renderObjects;

	Shader m_defaultShader;
};