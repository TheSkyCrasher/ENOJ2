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
	GameObject* monkey;
};

void Exiles::Init()
{
	plane = new GameObject(new Mesh("plane.obj"));
	bunny = new GameObject(new Mesh("bunny.obj"));
	monkey = new GameObject(new Mesh("monkey.obj"));

	monkey->GetTransform().SetPos(Vector3f(10.0f, 1.0f, 0.0f));
	monkey->GetTransform().SetRot(Quaternion(Vector3f(0,1,0),-1.57f));

	bunny->GetTransform().SetPos(Vector3f(2.0f, -0.1f, 2.0f));

	Camera* camera = new Camera(Matrix4f().InitPerspective(ToRadians(60.0f), Window::GetAspect(), 0.1f, 1000.0f));

	AddToScene(monkey);
	AddToScene(plane);
	AddToScene(bunny);
	SetCamera(camera);
}

void Exiles::Update()
{
	static float counter = 0.0f;

	monkey->GetTransform().SetRot(Quaternion(Vector3f(0, 1, 0), counter));

	counter += Timer::deltaTime();
}

int main(int argc, char** argv)
{
	std::cout.setf(std::ios::fixed);

	Window::Create(1024, 768, "Exiles");

	Exiles exiles;
	exiles.Start();

	return 0;
}
