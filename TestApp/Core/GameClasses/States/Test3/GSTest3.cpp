#include "GSTest3.h"
#include "../../../Game.h"
#include "../../Objects/Test3/Cube.h"

#define ORBCOUNT 1000

using namespace Bitz::Math;
using namespace Bitz::GFX;

std::shared_ptr<Cube> testCube = std::shared_ptr<Cube>(new Cube());

GSTest3::GSTest3() : Bitz::GameLogic::GameState("Main")
{
	testCube->SetSize(Vector3F(1.0f));
	testCube->SetColour(Vector4F(1.0f));
	testCube->SetVisible(true);
	testCube->SetTexture(Bitz::Content::ContentManager::Load<Texture>("box.Gdat"));
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

void GSTest3::OnUpdate()
{

}

void GSTest3::OnDraw()
{
	GraphicsManager::BeginRender(Game::RenderState3d);
	GraphicsManager::Render(testCube);
	GraphicsManager::EndRender();
}