#pragma once
#include "../../Common.h"

class GSMain : public Bitz::GameLogic::GameState
{
public:
	GSMain();
	~GSMain();
protected:
	virtual void OnEnter(GameState * preceedingState) override;
	virtual void OnExit() override;
	virtual void OnUpdate() override;
	virtual void OnDraw() override;
};

