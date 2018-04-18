#pragma once
#include "../../../Common.h"

#include "../../Objects/Test2/Phys_Crate.h"
#include "../../Objects/Test2/Phys_Ground.h"

class GSTest2 : public Bitz::GameLogic::GameState
{
public:
	GSTest2();
	~GSTest2();
protected:
	virtual void OnEnter(const Bitz::GameLogic::GameState_Ptr preceedingState) override;
	virtual void OnExit() override;
	virtual void OnUpdate(double ms) override;
	virtual void OnDraw() override;
private:
	b2World * _PhyWorld;
	Phys_Ground * _TestGround[50];
	Phys_Crate * _TestCreate[25];
};
