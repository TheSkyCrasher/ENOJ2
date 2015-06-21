#include "game.h"

class Exiles : public Game
{
public:
	Exiles() {}

	virtual void Init();
	virtual void Update();
};

void Exiles::Init()
{
	GameObject* terrain = new GameObject(new Mesh("Data/terrain.obj"));

	Camera* camera = new Camera(Matrix4f().InitPerspective(ToRadians(60.0f), Window::GetAspect(), 0.1f, 1000.0f));

	AddToScene(terrain);
	SetCamera(camera);
}

void Exiles::Update()
{

}

int main(int argc, char** argv)
{
	Exiles exiles;
	exiles.Start();

	return 0;
}
