#include "Game.h"

using namespace Bitz::Math;
using namespace Bitz::Content;
using namespace Bitz::GFX;

GraphicsStates::GS2D * Game::RenderState2d;

GSMain * Game::GameState_Main = nullptr;
GSPhysics * Game::GameState_Physics = nullptr;

Game::Game() : Bitz::GameLogic::GameCore("Explore")
{
	ContentManager::Init("Content/", "Content/", "Content/");
}

Game::~Game()
{
}

bool Game::Update()
{
	return true;
}

bool Game::Draw()
{
	return true;
}

bool Game::Init()
{
	Bitz::Debug::Logging::EnableLogToConsole();

	RenderState2d = new GraphicsStates::GS2D();
	RenderState2d->GetActiveCamera()->SetPosition(Vector3F(0, 0, 0));
	RenderState2d->SetActiveBlendState(Bitz::GFX::GraphicsStates::BaseGS::ADDATIVE);

	if (GameState_Main == nullptr)
	{
		GameState_Main = new GSMain();
		Bitz::GameLogic::GameStateService::StartState(GameState_Main);
	}
	if (GameState_Physics == nullptr)
	{
		GameState_Physics = new GSPhysics();
		Bitz::GameLogic::GameStateService::StartState(GameState_Physics);
	}
	return true;
}

bool Game::LoadContent()
{
	Phys_Ground::LoadContent();
	Phys_Crate::LoadContent();
	Orb::LoadContent();
	return true;
}

bool Game::UnloadContent()
{
	Phys_Ground::UnloadContent();
	Phys_Crate::UnloadContent();
	Orb::UnloadContent();
	return true;
}

bool Game::OnExit()
{
	Bitz::GameLogic::GameStateService::EndState(GameState_Main);
	delete GameState_Main;
	GameState_Main = nullptr;

	Bitz::GameLogic::GameStateService::EndState(GameState_Physics);
	delete GameState_Physics;
	GameState_Physics = nullptr;

	return true;
}