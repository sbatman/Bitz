#pragma once
#include "Common.h"

#include "GameClasses/States/GSMain.h"
#include "GameClasses/States/GSPhysics.h"

class Game : public Bitz::GameLogic::GameCore
{
public:
	Game();
	~Game();

	static Bitz::GFX::GraphicsStates::GS2D * RenderState2d;

protected:
	virtual bool Update() override;
	virtual bool Draw() override;
	virtual bool Init() override;
	virtual bool LoadContent() override;
	virtual bool UnloadContent() override;
	virtual bool OnExit() override;

private:
	static GSMain * GameState_Main;
	static GSPhysics * GameState_Physics;
};
