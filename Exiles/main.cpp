#include "game.h"

class Exiles : public Game
{
public:
	Exiles() {}

	virtual void Init();
	virtual void Update();
private:
	GameObject* podium;
};

void Exiles::Init()
{
	podium = new GameObject(new Mesh("podium.lwo"), new Texture("default_NRM.png"), new Texture("default_SPEC.png"));
	Camera* camera = new Camera(Matrix4f().InitPerspective(ToRadians(60.0f), Window::GetAspect(), 0.1f, 1000.0f));

	AddToScene(podium);
	SetCamera(camera);
}

void Exiles::Update()
{

}

int main(int argc, char** argv)
{
	std::cout.setf(std::ios::fixed);

	Window::Create(1024, 768, "Exiles");

	Exiles exiles;
	exiles.Start();

	return 0;
}
