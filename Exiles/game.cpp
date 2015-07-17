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
	m_defaultShader.AddUniform<Vector3f>("lightDirection");
	m_defaultShader.AddUniform<Vector3f>("cameraPosition");
	m_defaultShader.AddUniform<Matrix4f>("lightMV");
	m_defaultShader.AddUniform<Matrix4f>("MVP");
	m_defaultShader.AddUniform<Matrix4f>("MP");

	m_defaultShader.AddUniform<int>("diffuseTex");
	m_defaultShader.AddUniform<int>("normalTex");
	m_defaultShader.AddUniform<int>("specularTex");
	m_defaultShader.AddUniform<int>("depthTex");

	m_defaultShader.Bind();
	m_defaultShader.Update("lightDirection", m_light.GetDirection());
	m_defaultShader.Update("lightMV", m_light.GetMVP());
	m_defaultShader.Update("diffuseTex",  0);
	m_defaultShader.Update("normalTex",   1);
	m_defaultShader.Update("specularTex", 2);
	m_defaultShader.Update("depthTex",    3);

	Init();
	while (Window::IsOpen())
	{
		Input::Update();
		Timer::Update();
		m_mainCamera->Update(Timer::deltaTime());

		if (Input::GetKeyDown(Input::KEY_ESCAPE))
			Window::Close();

		Update();

		m_light.SetRender();
		for (unsigned int i = 0; i < m_objects.size(); ++i)
		{
			m_objects[i]->RenderMesh(m_light.GetShader());
		}

		Window::SetRenderToScreen();
		Window::Clear(0.0f, 0.3f, 0.6f, 1.0f);

		m_defaultShader.Bind();
		m_defaultShader.Update("cameraPosition", m_mainCamera->GetPos());
		m_light.BindTexture(3);

		for (unsigned int i = 0; i < m_objects.size(); ++i)
		{
			m_objects[i]->Draw(&m_defaultShader, m_mainCamera);
		}

		Window::Update();
	}
}