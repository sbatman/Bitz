#pragma once
#include "../../Common.h"
#include "../Camera.h"

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
				virtual ~BaseGS() {}
				virtual void EnterState() = 0;
				virtual void ExitState() = 0;
				virtual Camera * GetActiveCamera() const = 0;
				virtual bool IsNormalsEnabled() const = 0;

				void SetActiveBlendState(BlendStates newBlendState);
				BlendStates GetActiveBlendState();

			protected:
				BlendStates _CurrentBlendState;
			};
		}
	}
}