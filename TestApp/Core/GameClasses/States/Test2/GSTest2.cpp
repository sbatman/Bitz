#include "GSTest2.h"
#include "../../../Game.h"

using namespace Bitz::Math;
using namespace Bitz::GFX;

GSTest2::GSTest2() : Bitz::GameLogic::GameState("GSTest2")
{
}

GSTest2::~GSTest2()
{
}

void GSTest2::OnEnter(const Bitz::GameLogic::GameState_Ptr)
{
	_PhyWorld = new b2World(b2Vec2(0, 1));

	for (size_t i = 0; i < 30; i++)_TestGround[i] = new Phys_Ground(_PhyWorld, 32 * (i + 1), 768 - 32);
	for (size_t i = 30; i < 40; i++)_TestGround[i] = new Phys_Ground(_PhyWorld, 0, 768 - (32 * (i - 30)));
	for (size_t i = 40; i < 50; i++)_TestGround[i] = new Phys_Ground(_PhyWorld, 32 * 30, 768 - (32 * (i - 40)));
	for (size_t i = 0; i < 25; i++)_TestCreate[i] = new Phys_Crate(_PhyWorld, 24 * ((i % 6) + 20), 35 * i);
}

void GSTest2::OnExit()
{
	for (size_t i = 0; i < 50; i++) if (_TestGround[i] != nullptr)delete _TestGround[i];
	for (size_t i = 0; i < 25; i++)if (_TestCreate[i] != nullptr)delete _TestCreate[i];
	delete _PhyWorld;
}

void GSTest2::OnUpdate()
{
	_PhyWorld->Step(1.0f / 60.0f, 6, 2);

	for (size_t i = 0; i < 50; i++)_TestGround[i]->Update();

	for (size_t i = 0; i < 25; i++)_TestCreate[i]->Update();
}

void GSTest2::OnDraw()
{
	GraphicsManager::BeginRender(Game::RenderState2d);

	for (size_t i = 0; i < 50; i++)_TestGround[i]->Draw();

	for (size_t i = 0; i < 25; i++)_TestCreate[i]->Draw();
	GraphicsManager::EndRender();
}