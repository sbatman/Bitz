#pragma once

#include "../../Common.h"
#include "../../Interfaces/Core.h"
#include "IDrawable.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		class GraphicsManager;
		class RenderEngine;

		namespace Drawables
		{
			class Model : public Bitz::Interfaces::IPositionable3DF,
				public Bitz::Interfaces::ISizeable3DF,				
				public Bitz::Interfaces::IRotateable3DF,
				public IDrawable
			{
				friend GraphicsManager;
				friend RenderEngine;
			public:
				Model();
				~Model();

				virtual uint32_t GetVertCount() const override;

		
				virtual void SetColour(const Vector4F newColour) override;
				virtual void SetAlpha(const float newAlpha) override;

			

				virtual void SetNormalTexture(const Texture_Ptr newTexture);
				virtual void SetSpecularTexture(const Texture_Ptr newTexture);

				virtual Texture_Ptr GetNormalTexture() const;
				virtual Texture_Ptr GetSpecularTexture() const;

			protected:
				virtual void SetVertArray(const float_t * verts, const int32_t count);
				virtual void SetNormalArray(const float_t * norms, const  int32_t count);
				virtual void SetTexCordArray(const float_t * texcords, const  int32_t count);
				virtual void SetColourArray(const float_t * colours, const  int32_t count);
				virtual void PopulateVertArray(float_t * vertArray, int32_t * statPosition) override;
				virtual void PopulateColArray(float_t * colArray, int32_t * startPosition) override;
				virtual void PopulateTexArray(float_t * texArray, int32_t * startPosition) override;
				virtual void PopulateNormArray(float_t * normArray, int32_t * startPosition) override;

				Texture_Ptr _NormalTexture = nullptr;
				Texture_Ptr _SpecularTexture = nullptr;

				virtual glm::mat4 GetTransformation() const;

			private:
			};
		}
	}
}