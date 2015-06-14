#include "Phys_Crate.h"

using namespace Bitz::GFX;
using namespace Bitz::Content;
using namespace Bitz::Input;
using namespace Bitz::Math;

Texture* Phys_Crate::_DefaultTexture = nullptr;
b2PolygonShape *  Phys_Crate::_PhysShape = nullptr;

Phys_Crate::Phys_Crate(b2World * world, float_t x, float_t y) : I2DPhysicsEnabled(world, *_PhysShape)
{
	assert(GetDefaultTexture() != nullptr);
	SetPhysicsType(PhysicsType::DYNAMIC);
	_Sprite = new Drawables::Sprite();
	_Sprite->SetTexture(GetDefaultTexture());
	_Sprite->SetColour(Vector4F(1.0f));
	_Sprite->SetSize(Vector2F(32.0f));
	SetPosition(Vector2F(x, y));
	GetFixture()->SetFriction(1.0f);
	GetFixture()->SetRestitution(0.5f);
}

Phys_Crate::~Phys_Crate()
{
}

Texture* Phys_Crate::GetDefaultTexture()
{
	return _DefaultTexture;
}

void Phys_Crate::LoadContent()
{
	if (_DefaultTexture == nullptr)
	{
		_DefaultTexture = ContentManager::Load<Texture>("box.Gdat");
	}
	if (_PhysShape == nullptr)
	{
		_PhysShape = new b2PolygonShape();
		_PhysShape->SetAsBox(15, 15);
	}
}

void Phys_Crate::UnloadContent()
{
	delete _DefaultTexture;
	delete _PhysShape;
}

void Phys_Crate::Update()
{
	_Sprite->SetPosition(GetPosition());
	_Sprite->SetRotation(GetBody()->GetAngle());
	if (Keyboard::WasKeyReleased(VK_SPACE))
	{
		GetBody()->ApplyLinearImpulse(b2Vec2(0, -0.1f), GetBody()->GetWorldCenter(), true);
	}
}

void Phys_Crate::Draw()
{
	GraphicsManager::Render(_Sprite);
}