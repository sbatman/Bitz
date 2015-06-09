#include "Game.h"

using namespace Bitz::Math;
using namespace Bitz::Content;
using namespace Bitz::GFX;

Texture* Game::TestTexture = nullptr;
GraphicsStates::GS2D * Game::RenderState2d;
Drawables::Sprite * Game::TestSprite;

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
	TestSprite = new Drawables::Sprite();
	TestSprite->SetTexture(TestTexture);
	TestSprite->SetPosition(100, 100);
	TestSprite->SetSize(Vector2F(70, 70));

	/*if (GameState_Main == nullptr)
	{
		GameState_Main = new GSMain();
		Bitz::GameLogic::GameStateService::StartState(GameState_Main);
	}*/
	if (GameState_Physics == nullptr)
	{
		GameState_Physics = new GSPhysics();
		Bitz::GameLogic::GameStateService::StartState(GameState_Physics);
	}
	return true;
}

bool Game::LoadContent()
{
	if (TestTexture == nullptr)TestTexture = ContentManager::Load<Texture>("particle.Gdat");
	Phys_Ground::LoadContent();
	Phys_Crate::LoadContent();
	return true;
}

bool Game::UnloadContent()
{
	delete TestTexture;
	TestTexture = nullptr;
	return true;
}

bool Game::OnExit()
{
	//Bitz::GameLogic::GameStateService::EndState(GameState_Main);
	//delete GameState_Main;
	//GameState_Main = nullptr;

	Bitz::GameLogic::GameStateService::EndState(GameState_Physics);
	delete GameState_Physics;
	GameState_Physics = nullptr;

	return true;
}