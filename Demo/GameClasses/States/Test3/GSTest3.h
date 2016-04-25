#pragma once
#include "../../../Common.h"

class GSTest3 : public Bitz::GameLogic::GameState
{
public:
	GSTest3();
	~GSTest3();
protected:
	virtual void OnEnter(const Bitz::GameLogic::GameState_Ptr preceedingState) override;
	virtual void OnExit() override;
	virtual void OnUpdate(double ms) override;
	virtual void OnDraw() override;
};
