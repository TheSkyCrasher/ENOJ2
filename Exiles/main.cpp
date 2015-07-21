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
	GameObject* tree;
	GameObject* mitsuba;
};

void Exiles::Init()
{
	podium = new GameObject(new Mesh("podium.lxo"), new Texture("checkers.png"), new Texture("default_NRM.png"), new Texture("default_SPEC.png"));
	bunny = new GameObject(new Mesh("bunny.lxo"));
	tree = new GameObject(new Mesh("tree.lxo"));
	mitsuba = new GameObject(new Mesh("mitsuba.lxo"), new Texture("sand.png"), new Texture("sand_NRM.png"), new Texture("sand_SPEC.png"));

	mitsuba->GetTransform().SetPos(Vector3f(0.0f, 0.250f, -4.0f));
	bunny->GetTransform().SetPos(Vector3f(-2.0f, 0.11f, 1.0f));
	tree->GetTransform().SetPos(Vector3f(2.0f, 0.11f, 2.0f));
	AddToScene(new GameObject(new Mesh("plane.lxo"), new Texture("white.png")));

	Camera* camera = new Camera(Matrix4f().InitPerspective(ToRadians(60.0f), Window::GetAspect(), 0.1f, 1000.0f));

	AddToScene(podium);
	AddToScene(bunny);
	AddToScene(tree);
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
