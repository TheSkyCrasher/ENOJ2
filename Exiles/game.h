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
#include "gameObject.h"
#include "timer.h"
#include <vector>

class Game
{
public:
	Game() : m_defaultShader("testShader") {}
	virtual ~Game();

	void Start();

	virtual void Init() = 0;
	virtual void Update() = 0;

	void AddToScene(GameObject* gameObject) { m_objects.push_back(gameObject); }
	void SetCamera(Camera* camera) { m_mainCamera = camera; }
private:
	std::vector<GameObject*> m_objects;
	Camera* m_mainCamera;
	GLuint m_screenTexture;
	Shader m_defaultShader;
};