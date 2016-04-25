#pragma once
#include "../../../Common.h"

class Phys_Ground : public I2DPhysicsEnabled
{
public:
	Phys_Ground(b2World * world, float_t x, float_t y);
	~Phys_Ground();

	static void LoadContent();
	static void UnloadContent();

	void Update();
	void Draw();

private:
	static Bitz::GFX::Texture_Ptr _DefaultTexture;
	static b2PolygonShape * _PhysShape;

	static Bitz::GFX::Texture_Ptr GetDefaultTexture();

	Bitz::GFX::Drawables::Sprite_Ptr _Sprite = nullptr;
};
