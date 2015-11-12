#pragma once
#include "../../../Common.h"
#include "../../Objects/Test3/Background.h"

class GSTest3 : public Bitz::GameLogic::GameState
{
public:
	GSTest3();
	~GSTest3();
protected:
	virtual void OnEnter(Bitz::GameLogic::GameState_Ptr preceedingState) override;
	virtual void OnExit() override;
	virtual void OnUpdate() override;
	virtual void OnDraw() override;
};
