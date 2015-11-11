#include "GSTest1.h"
#include "../../../Game.h"

#define ORBCOUNT 30000

using namespace Bitz::Math;
using namespace Bitz::GFX;

static std::vector<std::shared_ptr<Orb>> _TestOrbs;

GSTest1::GSTest1() : Bitz::GameLogic::GameState("Main")
{
}

GSTest1::~GSTest1()
{
}

void GSTest1::OnEnter(GameState *)
{
	for (auto x = 0; x < ORBCOUNT; x++)
	{
		auto o = std::shared_ptr<Orb>(new Orb());
		o->SetColour(Vector4F(1.0f, (x % 150) / 255.0f, 0.0f, 0.05f));
		_TestOrbs.push_back(o);
	}
}

void GSTest1::OnExit()
{
	for (std::shared_ptr<Orb> o : _TestOrbs)o = nullptr;
}

void GSTest1::OnUpdate()
{
	int xLimit = Bitz::GFX::GraphicsManager::GetScreenSize().X;
	int yLimit = Bitz::GFX::GraphicsManager::GetScreenSize().Y;
	if (xLimit != 0 && yLimit != 0)
	{
		for (std::shared_ptr<Orb> o : _TestOrbs)o->Update(xLimit, yLimit, xLimit*0.5f, yLimit *0.5f);
	}
}

void GSTest1::OnDraw()
{
	GraphicsManager::BeginRender(Game::RenderState2d);
	for (std::shared_ptr<Orb> o : _TestOrbs) GraphicsManager::Render(o);
	GraphicsManager::EndRender();
}