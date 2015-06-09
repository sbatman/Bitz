#pragma once
#include "../../Common.h"

#include "../Objects/Phys_Crate.h"
#include "../Objects/Phys_Ground.h"

class GSPhysics : public Bitz::GameLogic::GameState
{
public:
	GSPhysics();
	~GSPhysics();
protected:
	virtual void OnEnter(GameState * preceedingState) override;
	virtual void OnExit() override;
	virtual void OnUpdate() override;
	virtual void OnDraw() override;
private:
	b2World * _PhyWorld;
	Phys_Ground * _TestGround[50];
	Phys_Crate * _TestCreate[25];
};

