#include "GSTest3.h"
#include "../../../Game.h"

#define ORBCOUNT 1000

using namespace Bitz::Math;
using namespace Bitz::GFX;

static Background_Ptr _Background;

GSTest3::GSTest3() : Bitz::GameLogic::GameState("Main")
{
}

GSTest3::~GSTest3()
{
}

void GSTest3::OnEnter(const Bitz::GameLogic::GameState_Ptr)
{
	_Background = Background_Ptr(new  Background());
	_Background->SetPosition(Vector2F(0));
}

void GSTest3::OnExit()
{
	_Background = nullptr;
}

void GSTest3::OnUpdate()
{
}

void GSTest3::OnDraw()
{
	GraphicsManager::BeginRender(Game::RenderState2d);
	GraphicsManager::Render(_Background);
	GraphicsManager::EndRender();
}