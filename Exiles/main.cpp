#include "game.h"
#include "stb_image.h"

class Exiles : public Game
{
public:
	Exiles() {}
	virtual ~Exiles() {}

	virtual void Init();
	virtual void Update();
private:
	StaticBody* scene01;
	StaticBody* skeleton;
};

void Exiles::Init()
{
	scene01 = new StaticBody(new Model("scene01.fbx"));
	scene01->GetModel()->SetCollisionShape(new btStaticPlaneShape(btVector3(0,1,0), 0));
	scene01->GetTransform().SetScale(0.01f);
	AddToScene(scene01);

	skeleton = new StaticBody(new Model("skeleton.dae"));
	skeleton->SetPos(-2.0f, 0.14f, 2.0f);
	skeleton->SetRot(0.0f, 1.0f, 0.0f, 0.0f);
	AddToRenderObjects(skeleton);

	SetCamera(new Camera(Matrix4f().InitPerspective(ToRadians(60.0f), Window::GetAspect(), 0.1f, 1000.0f)));
}

void Exiles::Update()
{
	if (Input::GetKeyDown(Input::KEY_ESCAPE))
	{
		Window::Close();
	}

	if (Input::GetKeyDown(Input::KEY_P))
		skeleton->GetModel()->ToggleAnimationPlaying();
}

int main(int argc, char** argv)
{
	std::cout.setf(std::ios::fixed);

	Window::Create(1024, 768, "Exiles", false);

	Exiles exiles;
	exiles.Start();

	return 0;
}
