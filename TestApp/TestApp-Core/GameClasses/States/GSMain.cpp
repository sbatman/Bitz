#include "GSMain.h"
#include "../../Game.h"

#define ORBCOUNT 1000

using namespace Bitz::Math;
using namespace Bitz::GFX;

static Orb *_TestOrbs[ORBCOUNT];

GSMain::GSMain() : Bitz::GameLogic::GameState("Main")
{
}

GSMain::~GSMain()
{
}

void GSMain::OnEnter(GameState *)
{
	for (auto x = 0; x < ORBCOUNT; x++)
	{
		_TestOrbs[x] = new Orb();
		_TestOrbs[x]->SetColour(Vector4F(1.0f, (x % 150) / 255.0f, 0.0f, 1.0f));
	}
}

void GSMain::OnExit()
{
	for (auto x = 0; x < ORBCOUNT; x++) delete _TestOrbs[x];
}

void GSMain::OnUpdate()
{
	int xLimit = Bitz::GFX::GraphicsManager::GetScreenSize().X;
	int yLimit = Bitz::GFX::GraphicsManager::GetScreenSize().Y;
	if (xLimit != 0 && yLimit != 0)
	{
		for (int x = 0; x < ORBCOUNT; x++)_TestOrbs[x]->Update(xLimit, yLimit, xLimit*0.5f, yLimit *0.5f);
	}
}

void GSMain::OnDraw()
{
	GraphicsManager::BeginRender(Game::RenderState2d);
	for (auto x = 0; x < ORBCOUNT; x++)GraphicsManager::Render(_TestOrbs[x]);
	GraphicsManager::EndRender();
}