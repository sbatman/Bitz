#pragma once
#include "../Interfaces.h"
#include "Box2D/Box2D.h"
#include "../Math/Math.h"

/// <summary>
/// This class acts as an easy to use interface for working with physics enabled objects within
/// the Box2d physics system
/// </summary>
class I2DPhysicsEnabled :
	public Bitz::Interfaces::IPositionable2DF,
	public Bitz::Interfaces::IRotateable2DF
{
public:
	enum PhysicsType
	{
		STATIC = b2_staticBody,
		KINEMATIC = b2_kinematicBody,
		DYNAMIC = b2_dynamicBody
	};

	I2DPhysicsEnabled(b2World * world, b2PolygonShape shape);
	~I2DPhysicsEnabled();

	PhysicsType GetPhysicsType();
	void SetPhysicsType(PhysicsType state);

	virtual Bitz::Math::Vector2F GetPosition() const override;
	virtual float_t GetRotation() const override;
	virtual void SetPosition(const Bitz::Math::Vector2F newPosition) override;
	virtual void SetRotation(const float_t newRotation) override;

	b2Fixture * GetFixture() const;
	b2Body* GetBody() const;

private:
	PhysicsType _PhysicsType = PhysicsType::STATIC;
	b2Fixture * _PhysFixture;
	b2Body * _PhysBody;
};
