#include "../../Common.h"
#include "BaseGS.h"

namespace Bitz
{
	namespace GFX
	{
		void GraphicsStates::BaseGS::SetActiveBlendState(BlendStates newBlendState)
		{
			_CurrentBlendState = newBlendState;
		}

		GraphicsStates::BaseGS::BlendStates GraphicsStates::BaseGS::GetActiveBlendState()
		{
			return _CurrentBlendState;
		}
	}
}