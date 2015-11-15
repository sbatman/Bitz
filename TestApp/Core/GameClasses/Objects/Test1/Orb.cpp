#include "Orb.h"

using namespace Bitz::GFX;
using namespace Bitz::Content;
using namespace Bitz::Input;
using namespace Bitz::Math;

Texture_Ptr Orb::_DefaultTexture = nullptr;

Orb::Orb()
{
	_Size = static_cast<float_t>((rand() % 50) + 20);
	SetSize(Vector2F(_Size));

	_PosX = (_Size*1.5f) + static_cast<float_t>(rand() % static_cast<int32_t>(1000 - (_Size * 3)));
	_PosY = (_Size*1.5f) + static_cast<float_t>(rand() % static_cast<int32_t>(1000 - (_Size * 3)));
	_AccX = ((rand() % 400) - 200) / 100.0f;
	_AccY = ((rand() % 400) - 200) / 100.0f;

	SetColour(Vector4F(1.0f, 1.0f, 1.0f, 0.1f));

	SetTexture(GetDefaultTexture());
}

Orb::~Orb()
{
}

void Orb::Update(int32_t xLimit, int32_t yLimit, int32_t targetX, int32_t targetY)
{
	targetPosx = targetX;
	targetPosy = targetY;

	float xRange;
	float yRange;

	if (Mouse::IsButtonDown(Mouse::MouseButtons::LEFT))
	{
		xRange = Mouse::GetX() - _PosX;
		yRange = Mouse::GetY() - _PosY;
	}
	else
	{
		xRange = _PosX - targetPosx;
		yRange = _PosY - targetPosy;
	}

	_AccX += xRange / 1000.0f;
	_AccY += yRange / 1000.0f;

	if (((_PosX - _AccX) + _Size) <= 0 || (_PosX - _AccX) >= (xLimit - _Size))
	{
		_AccX = -_AccX;
	}
	if (((_PosY - _AccY) + _Size) <= 0 || (_PosY - _AccY) >= (yLimit - _Size))
	{
		_AccY = -_AccY;
	}

	_PosX -= _AccX * (_Size / 40.0f);
	_PosY -= _AccY* (_Size / 40.0f);

	SetPosition(_PosX, _PosY);

	if (((xRange*xRange) + (yRange*yRange)) > (500 * 500))
	{
		_AccX *= 0.98f;
		_AccY *= 0.98f;
	}
}

void Orb::LoadContent()
{
	if (_DefaultTexture == nullptr)
	{
		_DefaultTexture = ContentManager::Load<Texture>("particle.Gdat");
	}
}

void Orb::UnloadContent()
{
	_DefaultTexture = nullptr;
}

Texture_Ptr Orb::GetDefaultTexture()
{
	return _DefaultTexture;
}