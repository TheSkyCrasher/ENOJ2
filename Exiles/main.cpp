#include "game.h"
#include "physics.h"

class Exiles : public Game
{
public:
	Exiles() {}

	virtual void Init();
	virtual void Update();
private:
	StaticBody* scene01;
};

void Exiles::Init()
{
	scene01 = new StaticBody(new Mesh("scene01.fbx"), new btStaticPlaneShape(btVector3(0, 1, 0), 0));
	scene01->GetTransform().SetScale(0.01f);

	int c = 0;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				AddToScene(new RigidBody(new Mesh("ball.lwo"), new btSphereShape(0.32f), 1.0f, Vector3f(-1.0f + i / 2.0f, 20.0f - k / 2.0f, 9.0f + j / 2.0f)));
				++c;
			}
		}
	}

	AddToScene(scene01);
	SetCamera(new Camera(Matrix4f().InitPerspective(ToRadians(60.0f), Window::GetAspect(), 0.1f, 1000.0f)));
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
