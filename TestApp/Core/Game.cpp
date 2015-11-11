#include "Game.h"

using namespace Bitz::Math;
using namespace Bitz::Content;
using namespace Bitz::GFX;

GraphicsStates::GS2D * Game::RenderState2d;

GSTest1 * Game::GameState_Test1 = nullptr;
GSTest2 * Game::GameState_Test2 = nullptr;
GSTest3 * Game::GameState_Test3 = nullptr;

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
	Bitz::Debug::Logging::EnableLogToBlackHole();

	RenderState2d = new GraphicsStates::GS2D();
	RenderState2d->GetActiveCamera()->SetPosition(Vector3F(0, 0, 0));
	RenderState2d->SetActiveBlendState(Bitz::GFX::GraphicsStates::BaseGS::ADDATIVE);
	

	if (GameState_Test1 == nullptr)
	{
		GameState_Test1 = new GSTest1();
		Bitz::GameLogic::GameStateService::StartState(GameState_Test1);
	}
	
	/*if (GameState_Test2 == nullptr)
	{
		GameState_Test2 = new GSTest2();
		Bitz::GameLogic::GameStateService::StartState(GameState_Test2);
	}*/

	//if (GameState_Test3 == nullptr)
	//{
	//	GameState_Test3 = new GSTest3();
	//	Bitz::GameLogic::GameStateService::StartState(GameState_Test3);
	//}
	return true;
}

bool Game::LoadContent()
{
	Phys_Ground::LoadContent();
	Phys_Crate::LoadContent();
	Orb::LoadContent();
	Background::LoadContent();
	return true;
}

bool Game::UnloadContent()
{
	Phys_Ground::UnloadContent();
	Phys_Crate::UnloadContent();
	Orb::UnloadContent();
	Background::UnloadContent();
	return true;
}

bool Game::OnExit()
{
	if (GameState_Test1 != nullptr)
	{
		Bitz::GameLogic::GameStateService::EndState(GameState_Test1);
		delete GameState_Test1;
		GameState_Test1 = nullptr;
	}
	
	if (GameState_Test2 != nullptr)
	{
		Bitz::GameLogic::GameStateService::EndState(GameState_Test2);
		delete GameState_Test2;
		GameState_Test2 = nullptr;
	}

	if (GameState_Test3 != nullptr)
	{
		Bitz::GameLogic::GameStateService::EndState(GameState_Test3);
		delete GameState_Test3;
		GameState_Test3 = nullptr;
	}

	return true;
}