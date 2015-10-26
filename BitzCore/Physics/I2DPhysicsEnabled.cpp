#include "I2DPhysicsEnabled.h"

using namespace Bitz::Math;

const float_t PhysicsPixelsToMeter = 100;
const float_t PhysicsMeterToPixels = 1.0f / PhysicsPixelsToMeter;

I2DPhysicsEnabled::I2DPhysicsEnabled(b2World * world, b2PolygonShape shape)
{
	b2PolygonShape internalShape = b2PolygonShape(shape);
	for (size_t i = 0; i < 8; i++)internalShape.m_vertices[i] *= PhysicsMeterToPixels;
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	_PhysBody = world->CreateBody(&groundBodyDef);
	_PhysFixture = _PhysBody->CreateFixture(&internalShape, 1);
}

I2DPhysicsEnabled::~I2DPhysicsEnabled()
{
	if (_PhysBody != nullptr)
	{
		b2World* world = _PhysBody->GetWorld();
		if (_PhysFixture != nullptr) _PhysBody->DestroyFixture(_PhysFixture);
		_PhysFixture = nullptr;
		world->DestroyBody(_PhysBody);
		_PhysBody = nullptr;
	}
}

Vector2F I2DPhysicsEnabled::GetPosition() const
{
	if (_PhysBody == nullptr) return _Position;
	return Vector2F(_PhysBody->GetPosition()) * PhysicsPixelsToMeter;
}

void I2DPhysicsEnabled::SetPosition(const Vector2F newPosition)
{
	_Position = newPosition;
	if (_PhysBody != nullptr)_PhysBody->SetTransform(b2Vec2(newPosition.X*PhysicsMeterToPixels, newPosition.Y*PhysicsMeterToPixels), _PhysBody->GetAngle());
}

I2DPhysicsEnabled::PhysicsType I2DPhysicsEnabled::GetPhysicsType()
{
	return _PhysicsType;
}

void I2DPhysicsEnabled::SetPhysicsType(PhysicsType state)
{
	_PhysicsType = state;
	if (_PhysBody != nullptr)_PhysBody->SetType(static_cast<b2BodyType>(state));
}

b2Fixture * I2DPhysicsEnabled::GetFixture() const
{
	return _PhysFixture;
}

b2Body* I2DPhysicsEnabled::GetBody() const
{
	return _PhysBody;
}

float_t I2DPhysicsEnabled::GetRotation() const
{
	if (_PhysBody == nullptr) return _Rotation;
	return _PhysBody->GetAngle();
}

void I2DPhysicsEnabled::SetRotation(const float_t newRotation)
{
	_Rotation = newRotation;
	if (_PhysBody != nullptr)_PhysBody->SetTransform(GetBody()->GetPosition(), newRotation);
}