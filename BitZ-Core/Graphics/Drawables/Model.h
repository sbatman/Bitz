#pragma once
#include "../../Common.h"
#include "../../Interfaces.h"
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

				virtual void SetVerts(const float_t * verts, const int32_t count);
				virtual void SetNormals(const float_t * norms, const  int32_t count);
				virtual void SetTexCords(const float_t * texcords, const  int32_t count);

				virtual void PopulateVertArray(float_t * vertArray, int32_t * statPosition) override;
				virtual void PopulateColArray(float_t * colArray, int32_t * startPosition) override;
				virtual void PopulateTexArray(float_t * texArray, int32_t * startPosition) override;
				virtual void PopulateNormArray(float_t * normArray, int32_t * startPosition) override;

			protected:

				virtual void ApplyTransformation();

			private:
			};
		}
	}
}