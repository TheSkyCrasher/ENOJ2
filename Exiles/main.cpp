#include "game.h"

class Exiles : public Game
{
public:
	Exiles() {}

	virtual void Init();
	virtual void Update();
private:
	GameObject* podium;
	GameObject* bunny;
	GameObject* mitsuba;
};

void Exiles::Init()
{
	podium = new GameObject(new Mesh("podium.obj"), new Texture("checkers.png"), new Texture("default_NRM.png"), new Texture("default_SPEC.png"));
	bunny = new GameObject(new Mesh("bunny.obj"));
	mitsuba = new GameObject(new Mesh("mitsuba.obj"), new Texture("sand.png"), new Texture("sand_NRM.png"), new Texture("sand_SPEC.png"));

	mitsuba->GetTransform().SetPos(Vector3f(0.0f, 0.25f, -4.0f));
	podium->GetTransform().SetPos(Vector3f(0.0f, 0.25f, 0.0f));
	bunny->GetTransform().SetPos(Vector3f(2.0f, 0.15f, 2.0f));

	Vertex vertices[] = {
		Vertex(-5.01f, 0.25001f, -5.01f, 0.0f, 20.0f, 0.0f, 1.0f, 0.0f),
		Vertex(-5.01f, 0.25001f, 5.01f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex(5.01f, 0.25001f, 5.01f, 20.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex(5.01f, 0.25001f, -5.01f, 20.0f, 20.0f, 0.0f, 1.0f, 0.0f),
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	AddToScene(new GameObject(new Mesh("plane.obj"), new Texture("white.png")));
	AddToScene(new GameObject(new Mesh(vertices, 4, indices, 6), new Texture("moss_01_co.png")));

	Camera* camera = new Camera(Matrix4f().InitPerspective(ToRadians(60.0f), Window::GetAspect(), 0.1f, 1000.0f));

	AddToScene(podium);
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
