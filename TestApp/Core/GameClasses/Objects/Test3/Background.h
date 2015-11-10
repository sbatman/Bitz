#include "../../../Common.h"

class Background : public Bitz::GFX::Drawables::Sprite
{
public:
	Background();
	~Background();

	static void LoadContent();
	static void UnloadContent();

private:
	static Bitz::GFX::Texture_Ptr _DefaultTexture;
	static Bitz::GFX::Texture_Ptr GetDefaultTexture();
};

typedef std::shared_ptr<Background> Background_Ptr;
