#include "Game.h"

using namespace Bitz::Math;
using namespace Bitz::Content;
using namespace Bitz::GFX;

GraphicsStates::GS2D * Game::RenderState2d;
GraphicsStates::GS3D * Game::RenderState3d;

std::shared_ptr<GSTest1> Game::GameState_Test1 = nullptr;
std::shared_ptr<GSTest2> Game::GameState_Test2 = nullptr;
std::shared_ptr<GSTest3> Game::GameState_Test3 = nullptr;

Game::Game() : Bitz::GameLogic::GameCore("Explore")
{
	ContentManager::Init("Content/", "Content/", "Content/");
	Bitz::Debug::Logging::EnableLogToConsole();
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

	RenderState3d = new GraphicsStates::GS3D();
	RenderState3d->GetActiveCamera()->SetPosition(Vector3F(0, 0, -20));
	RenderState3d->SetActiveBlendState(Bitz::GFX::GraphicsStates::BaseGS::ALPHA);	


	if (GameState_Test3 == nullptr)
	{
		GameState_Test3 = std::make_shared<GSTest3>();
		Bitz::GameLogic::GameStateService::StartState(GameState_Test3);
	}

	if (GameState_Test1 == nullptr)
	{
		GameState_Test1 = std::make_shared<GSTest1>();
		Bitz::GameLogic::GameStateService::StartState(static_cast<Bitz::GameLogic::GameState_Ptr>(GameState_Test1));
	}
	
	/*if (GameState_Test2 == nullptr)
	{
		GameState_Test2 = std::make_shared<GSTest2>();
		Bitz::GameLogic::GameStateService::StartState(GameState_Test2);
	}*/

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
	if (GameState_Test1 != nullptr)
	{
		Bitz::GameLogic::GameStateService::EndState(GameState_Test1);
		GameState_Test1 = nullptr;
	}
	
	if (GameState_Test2 != nullptr)
	{
		Bitz::GameLogic::GameStateService::EndState(GameState_Test2);
		GameState_Test2 = nullptr;
	}

	if (GameState_Test3 != nullptr)
	{
		Bitz::GameLogic::GameStateService::EndState(GameState_Test3);
		GameState_Test3 = nullptr;
	}

	return true;
}