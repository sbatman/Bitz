#include "GSTest3.h"
#include "../../../Game.h"

#define ORBCOUNT 1000

using namespace Bitz::Math;
using namespace Bitz::GFX;


GSTest3::GSTest3() : Bitz::GameLogic::GameState("Main")
{
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
//	GraphicsManager::Render();
	GraphicsManager::EndRender();
}