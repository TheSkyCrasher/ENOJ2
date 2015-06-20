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

int main(int argc, char** argv)
{
	Window::Create(800, 600, "Exiles");

	Shader shader("Data/testShader");
	GameObject* terrain = new GameObject(new Mesh("Data/terrain.obj"), new Texture("Data/default.png"));
	Camera camera(Matrix4f().InitPerspective(ToRadians(60.0f), Window::GetAspect(), 0.1f, 1000.0f));

	float counter = 0.0f;
	while (Window::IsOpen())
	{
		Window::Clear(0.0f, 0.15f, 0.3f, 1.0f);
		Input::Update();
		Timer::Update();
		camera.Update(Timer::deltaTime());

		if (Input::GetKeyDown(Input::KEY_ESCAPE))
			Window::Close();

		terrain->Draw(&shader, &camera);

		Window::Update();
	}

	if (terrain) delete terrain;

	return 0;
}
