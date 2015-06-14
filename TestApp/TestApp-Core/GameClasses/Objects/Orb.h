#include "../../Common.h"

class Orb : public Bitz::GFX::Drawables::Sprite
{
public:
	Orb();
	~Orb();
	void Update(int32_t xLimit, int32_t yLimit, int32_t targetX, int32_t targetY);

	static void LoadContent();
	static void UnloadContent();

private:
	static Bitz::GFX::Texture * _DefaultTexture;

	float_t _PosX = 0;
	float_t _PosY = 0;
	float_t _AccX = 0.9f;
	float_t _AccY = 0.7f;
	float_t _Size = 0;
	int targetPosx = 0;
	int targetPosy = 0;

	static Bitz::GFX::Texture * GetDefaultTexture();
};
