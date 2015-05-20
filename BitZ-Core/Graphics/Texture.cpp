#include "../Common.h"
#include "../Math/Math.h"
#include "../Content/TextureData.h"
#include "Texture.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		Texture::Texture()
		{
			_UV_TL = Vector2S(0, 0);
			_UV_TR = Vector2S(1, 0);
			_UV_BL = Vector2S(0, 1);
			_UV_BR = Vector2S(1, 1);
		}

		Texture::~Texture()
		{
			_Data->DecrementUsageCount();
			_Data = nullptr;
		}

		Vector2F Texture::GetSourceSize() const
		{
			return Vector2F(GetSourceWidth(), GetSourceHeight());
		}

		float_t Texture::GetSourceHeight() const
		{
			return static_cast<float_t>(_Data->GetHeight());
		}

		float_t Texture::GetSourceWidth() const
		{
			return static_cast<float_t>(_Data->GetWidth());
		}

		Vector2F Texture::GetSize() const
		{
			return Vector2F(GetWidth(), GetHeight());
		}

		float_t Texture::GetHeight() const
		{
			return float_t((_UV_BL.Y - _UV_TL.Y)*GetSourceHeight());
		}

		float_t Texture::GetWidth() const
		{
			return float_t((_UV_TR.X - _UV_TL.X)*GetSourceWidth());
		}

		void Texture::SetUVCoords(const Vector2S tl, const Vector2S tr, const Vector2S bl, const Vector2S br)
		{
			_UV_TL = tl;
			_UV_TR = tr;
			_UV_BL = bl;
			_UV_BR = br;
		}

		Texture* GFX::Texture::Load(std::string const fileName)
		{
			Content::TextureData * data = Content::TextureData::Load(fileName);
			GFX::Texture * returnTexture = new Texture();
			returnTexture->_Data = data;
			returnTexture->_Data->IncrementUsageCount();
			return returnTexture;
		}
	}
}