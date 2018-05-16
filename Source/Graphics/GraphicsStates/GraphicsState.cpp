#include "../../Common.h"
#include "GraphicsState.h"

namespace Bitz
{
	namespace GFX
	{
		void GraphicsStates::GraphicsState::SetActiveBlendState(BlendStates newBlendState)
		{
			_CurrentBlendState = newBlendState;
		}

		GraphicsStates::GraphicsState::BlendStates GraphicsStates::GraphicsState::GetActiveBlendState()
		{
			return _CurrentBlendState;
		}

		Shaders::Shader_Ptr GraphicsStates::GraphicsState::GetStockShader()
		{
			return _StockShader;
		}
	}
}