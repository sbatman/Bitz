#pragma once
#include "../../Common.h"
#include "../Camera.h"
#include "../Shaders/Shader.h"

namespace Bitz
{
	namespace GFX
	{
		namespace GraphicsStates
		{
			class BaseGS
			{
			public:
				enum BlendStates { NONE, ALPHA, ADDATIVE };
				virtual ~BaseGS() = default;
				virtual void EnterState() = 0;
				virtual void ExitState() = 0;
				virtual Camera_Ptr GetActiveCamera() const = 0;
				virtual bool IsNormalsEnabled() const = 0;

				void SetActiveBlendState(BlendStates newBlendState);
				BlendStates GetActiveBlendState();
				Shaders::Shader_Ptr GetStockShader();

			protected:
				BlendStates _CurrentBlendState = BlendStates::NONE;
				Shaders::Shader_Ptr _StockShader;
			};
		}
	}
}