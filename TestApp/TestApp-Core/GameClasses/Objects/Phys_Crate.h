#pragma once
#include "../../Common.h"

class Phys_Crate : public I2DPhysicsEnabled
{
public:
	Phys_Crate(b2World * world, float_t x, float_t y);
	~Phys_Crate();

	static void LoadContent();
	static void UnloadContent();

	void Update();
	void Draw();

private:
	static Bitz::GFX::Texture * _DefaultTexture;
	static b2PolygonShape * _PhysShape;

	Bitz::GFX::Drawables::Sprite * _Sprite = nullptr;

	static Bitz::GFX::Texture * GetDefaultTexture();
};
