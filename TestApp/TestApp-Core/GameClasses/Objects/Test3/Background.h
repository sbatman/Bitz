#include "../../../Common.h"

class Background : public Bitz::GFX::Drawables::Sprite
{
public:
	Background();
	~Background();

	static void LoadContent();
	static void UnloadContent();

private:
	static Bitz::GFX::Texture * _DefaultTexture;
	static Bitz::GFX::Texture * GetDefaultTexture();
};
