#include "game.h"
#include <ctime>
#include "util.h"

Game::~Game()
{
	Physics::Delete();

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
	srand((unsigned int) time(0));

	Matrix4f lightMVP = m_light.GetMVP();

	m_defaultShader.Bind();
	m_defaultShader.SetUniform("lightDirection", m_light.GetDirection());
	m_defaultShader.SetUniform("lightMVP", lightMVP);
	m_defaultShader.SetUniform("depthTex", 0);
	m_defaultShader.SetUniform("diffuseTex", 1);
	m_defaultShader.SetUniform("normalTex", 2);
	m_defaultShader.SetUniform("specularTex", 3);

	Physics::Init(9.8f);

	Init();
	unsigned int renderObjects = m_objects.size();
	while (Window::IsOpen())
	{
		Input::Update();
		Timer::Update();
		float dt = Timer::deltaTime();
		Physics::Update(dt);

		m_mainCamera->Update(Timer::deltaTime());

		if (Input::GetKeyDown(Input::KEY_ESCAPE))
		{
			Window::Close();
		}

		if (Input::GetMouseDown(Input::LEFT_MOUSE))
		{
			AddToScene(new RigidBody(new Mesh("ball.lwo"), new btSphereShape(0.32f), 1.0f, m_mainCamera->GetPos()));
			m_objects[renderObjects]->GetPhysicsBody()->applyImpulse(btVector3(m_mainCamera->GetDirection().GetX(), m_mainCamera->GetDirection().GetY(), m_mainCamera->GetDirection().GetZ())*30.0f,
				btVector3(0, 0, 0));
			renderObjects++;
		}

		Update();
		static float x = 0.0f;
		m_light.SetRender();
		for (unsigned int i = 0; i < renderObjects; ++i)
		{
			m_objects[i]->RenderMesh(m_light.GetShader());
		}
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		Window::SetRenderToScreen();
		Window::Clear(0.0f, 0.3f, 0.6f, 1.0f);

		m_skybox.Draw(m_mainCamera);
		
		glDisable(GL_CULL_FACE);
		m_defaultShader.Bind();
		m_defaultShader.SetUniform("cameraPosition", m_mainCamera->GetPos());
		m_light.BindTexture(0);
		for (unsigned int i = 0; i < renderObjects; ++i)
		{
			m_objects[i]->Draw(&m_defaultShader, m_mainCamera);
		}

		m_grass.Draw(m_mainCamera, lightMVP);

		Window::Update();
	}
}