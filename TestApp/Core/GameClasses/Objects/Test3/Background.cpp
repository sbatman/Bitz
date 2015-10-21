#include "Background.h"

using namespace Bitz::GFX;
using namespace Bitz::Content;
using namespace Bitz::Input;
using namespace Bitz::Math;

Texture_Ptr Background::_DefaultTexture = nullptr;

Background::Background()
{
	SetSize(Vector2F(1024));

	SetColour(Vector4F(1.0f, 1.0f, 1.0f, 1.0f));

	SetTexture(GetDefaultTexture());
}

Background::~Background()
{
}

void Background::LoadContent()
{
	if (_DefaultTexture == nullptr)
	{
		_DefaultTexture = ContentManager::Load<Texture>("purple.Gdat");
	}
}

void Background::UnloadContent()
{
	_DefaultTexture = nullptr;
}

Texture_Ptr Background::GetDefaultTexture()
{
	return _DefaultTexture;
}