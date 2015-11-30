#pragma once
#include "../../../Common.h"
#include "../../Objects/Test1/Orb.h"

class GSTest1 : public Bitz::GameLogic::GameState
{
public:
	GSTest1();
	~GSTest1();
protected:
	virtual void OnEnter(const Bitz::GameLogic::GameState_Ptr preceedingState) override;
	virtual void OnExit() override;
	virtual void OnUpdate(double ms) override;
	virtual void OnDraw() override;
};
