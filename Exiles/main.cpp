#include "game.h"
#include "stb_image.h"

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
	scene01 = new StaticBody(new Mesh("scene01.fbx"));
	scene01->SetCollision(new btStaticPlaneShape(btVector3(0,1,0), 0));
	scene01->GetTransform().SetScale(0.01f);

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
