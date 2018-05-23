#pragma once
#pragma once
#include "../../Common.h"
#include "../../Math/Math.h"
#include "../../Interfaces/Core.h"
#include "../Shaders/Shader.h"
#include "../Camera.h"


namespace Bitz
{
	namespace GFX
	{
		class GraphicsManager;
		class RenderEngine;

		namespace Effects
		{
			class Light : public Bitz::Interfaces::IPositionable3DF
			{
				friend GraphicsManager;
				friend RenderEngine;

			public:
				Light();
				void SetRadius(float_t radius);
				void SetColour(const Bitz::Vector3F& newColour);
				Bitz::Vector3F GetColour() const;
			private:
				float_t _Linear;
				float_t _Quadratic;
				Bitz::Vector3F _Colour;
				void SetupShader(const Bitz::GFX::Shaders::Shader_Ptr& shader, int lightID) const;
			};
			typedef std::shared_ptr<Light> Light_Ptr;
		}
	}
}