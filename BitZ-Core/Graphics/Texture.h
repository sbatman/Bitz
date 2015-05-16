#pragma once
#include "../Common.h"
#include "../Math/Math.h"
#include "../Content/TextureData.h"

namespace Bitz
{
	namespace Content
	{
		class ContentManager;
	}

	using namespace Math;

	namespace GFX
	{
		namespace Drawables
		{
			class Sprite;
		}

		class GraphicsManager;
		class RenderEngine;

		class Texture
		{
			friend Drawables::Sprite;
			friend Content::ContentManager;
			friend GraphicsManager;
			friend RenderEngine;
		public:
			Texture();
			~Texture();

			void SetUVCoords(const Vector2S tl, const Vector2S tr, const Vector2S bl, const Vector2S br);

			Vector2F GetSourceSize() const;
			float_t GetSourceHeight() const;
			float_t GetSourceWidth() const;
			Vector2F GetSize() const;
			float_t GetHeight() const;
			float_t GetWidth() const;

		protected:

			static Texture * Load(const std::wstring fileName);
			Vector2S _UV_TL;
			Vector2S _UV_TR;
			Vector2S _UV_BL;
			Vector2S _UV_BR;

		private:

			Content::TextureData* _Data;
		};
	}
}