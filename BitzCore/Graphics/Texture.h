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
		class Texture;
		class GraphicsManager;
		class RenderEngine;

		typedef std::shared_ptr<Texture> Texture_Ptr;

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

			static Texture_Ptr Load(const std::string fileName);
			Vector2S _UV_TL;
			Vector2S _UV_TR;
			Vector2S _UV_BL;
			Vector2S _UV_BR;

		private:

			Content::TextureData_Ptr _Data;
		};
	}
}