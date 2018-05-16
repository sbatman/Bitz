#pragma once
#include "../../Common.h"
#include "GraphicsState.h"

namespace Bitz
{
	namespace GFX
	{
		namespace GraphicsStates
		{
			class GS2D : public GraphicsState
			{
			public:
				GS2D();
				void EnterState() override;
				void ExitState() override;
				bool IsNormalsEnabled() const override;
				Camera_Ptr GetActiveCamera() const override;
			private:
				Camera_Ptr _ActiveCamera;
			};
		}
	}
}
