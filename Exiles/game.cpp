#include "game.h"

Game::~Game()
{
	for (unsigned int i = 0; i < m_objects.size(); ++i)
	{
		if (m_objects[i])
			delete m_objects[i];
	}

	if (m_mainCamera) delete m_mainCamera;

	std::cout << "CLEARED" << std::endl;
}

void Game::Start()
{
	Init();
	while (Window::IsOpen())
	{
		Window::Clear(0.0f, 0.15f, 0.3f, 1.0f);
		Input::Update();
		Timer::Update();
		m_mainCamera->Update(Timer::deltaTime());

		if (Input::GetKeyDown(Input::KEY_ESCAPE))
			Window::Close();

		Update();

		for (unsigned int i = 0; i < m_objects.size(); ++i)
		{
			m_objects[i]->RenderMesh(&m_defaultShader, m_mainCamera);
		}

		Window::Update();
	}
}