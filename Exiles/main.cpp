#include "game.h"

class Exiles : public Game
{
public:
	Exiles() {}

	virtual void Init();
	virtual void Update();
private:
	GameObject* plane;
	GameObject* bunny;
	GameObject* mitsuba;
};

void Exiles::Init()
{
	plane = new GameObject(new Mesh("plane.obj"), new Texture("checkers.png"), new Texture("default_NRM.png"), new Texture("default_SPEC.png"));
	bunny = new GameObject(new Mesh("bunny.obj"));
	mitsuba = new GameObject(new Mesh("mitsuba.obj"), new Texture("sand.png"), new Texture("sand_NRM.png"));

	mitsuba->GetTransform().SetPos(Vector3f(0.0f, 0.0f, -4.0f));

	bunny->GetTransform().SetPos(Vector3f(2.0f, -0.1f, 2.0f));

	Camera* camera = new Camera(Matrix4f().InitPerspective(ToRadians(60.0f), Window::GetAspect(), 0.1f, 1000.0f));

	AddToScene(plane);
	AddToScene(bunny);
	AddToScene(mitsuba);
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
