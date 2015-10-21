#pragma once
#include "../../Common.h"
#include "../../Interfaces.h"
#include "../Texture.h"
#include "IDrawable.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		namespace Drawables
		{
			class Sprite : public Bitz::Interfaces::IPositionable2DF,
				public Bitz::Interfaces::ISizeable2DF,
				public Bitz::Interfaces::IRotateable2DF,
				public IDrawable
			{
			public:
				Sprite();
				~Sprite();
				virtual uint32_t GetVertCount() const override;

				virtual void SetPosition(const Vector2F newPosition) override;
				virtual void SetPosition(const float x, const float y);
				virtual void SetSize(const Vector2F newSize) override;
				virtual void SetTexture(const Texture_Ptr texture) override;
				virtual void SetRotation(const float_t newRotation) override;

				virtual void PopulateVertArray(float_t * vertArray, int32_t * statPosition) override;
				virtual void PopulateTexArray(float_t * texArray, int32_t * statPosition) override;
				virtual void PopulateColArray(float_t* colArray, int32_t* startPosition) override;
				virtual void PopulateNormArray(float_t* normArray, int32_t* startPosition) override;

			protected:
				virtual void UpdateQuad();
				virtual void FlagQuadDirty();

			private:
				bool _QuadDirty = true;
			};

			typedef std::shared_ptr<Sprite> Sprite_Ptr;
		}
	}
}