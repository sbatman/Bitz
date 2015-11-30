#include "GSTest3.h"
#include "../../../Game.h"
#include "../../Objects/Test3/Cube.h"

#define ORBCOUNT 1000

using namespace Bitz::Math;
using namespace Bitz::GFX;

std::shared_ptr<Cube> testCube = std::shared_ptr<Cube>(new Cube());
std::shared_ptr<Cube> testCube1 = std::shared_ptr<Cube>(new Cube());
std::shared_ptr<Cube> testCube2 = std::shared_ptr<Cube>(new Cube());

GSTest3::GSTest3() : Bitz::GameLogic::GameState("Main")
{
	testCube->SetVisible(true);
	testCube->SetPosition(Vector3F(0, -1, 0));
	testCube->SetTexture(Bitz::Content::ContentManager::Load<Texture>("box.Gdat"));

	testCube1->SetVisible(true);
	testCube1->SetPosition(Vector3F(-1, -1, 0));			
	testCube1->SetTexture(Bitz::Content::ContentManager::Load<Texture>("box.Gdat"));

	testCube2->SetVisible(true);
	testCube2->SetPosition(Vector3F(-2, -1, 0));			
	testCube2->SetTexture(Bitz::Content::ContentManager::Load<Texture>("box.Gdat"));
}

GSTest3::~GSTest3()
{
}

void GSTest3::OnEnter(const Bitz::GameLogic::GameState_Ptr)
{

}

void GSTest3::OnExit()
{
}

void GSTest3::OnUpdate(double ms)
{
	
}

void GSTest3::OnDraw()
{
	GraphicsManager::BeginRender(Game::RenderState3d);	

	GraphicsManager::Render(testCube);
	GraphicsManager::Render(testCube1);
	GraphicsManager::Render(testCube2);
	GraphicsManager::EndRender();
}