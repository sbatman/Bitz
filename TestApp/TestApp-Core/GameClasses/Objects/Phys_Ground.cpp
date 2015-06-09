#include "Phys_Ground.h"

using namespace Bitz::GFX;
using namespace Bitz::Content;
using namespace Bitz::Input;
using namespace Bitz::Math;

Texture* Phys_Ground::_DefaultTexture = nullptr;
b2PolygonShape *  Phys_Ground::_PhysShape = nullptr;

Phys_Ground::Phys_Ground(b2World * world, float_t x, float_t y) : I2DPhysicsEnabled(world, *_PhysShape)
{
	assert(GetDefaultTexture() != nullptr);
	_Sprite = new Drawables::Sprite();
	_Sprite->SetTexture(GetDefaultTexture());
	_Sprite->SetColour(Vector4F(1.0f));
	_Sprite->SetSize(Vector2F(32.0f));
	SetPosition(Vector2F(x,y));
}

Phys_Ground::~Phys_Ground()
{

}

Texture* Phys_Ground::GetDefaultTexture()
{
	return _DefaultTexture;
}

void Phys_Ground::LoadContent()
{
	if (_DefaultTexture == nullptr)
	{
		_DefaultTexture = ContentManager::Load<Texture>("box.Gdat");
	}
	if (_PhysShape == nullptr)
	{
		_PhysShape = new b2PolygonShape();
		_PhysShape->SetAsBox(16,16);
	}
}

void Phys_Ground::UnloadContent()
{
	delete _DefaultTexture;
	delete _PhysShape;
}

void Phys_Ground::Update()
{
	_Sprite->SetPosition(GetPosition());
}

void Phys_Ground::Draw()
{
	GraphicsManager::Render(_Sprite);
}