#pragma once
#include "../../Common.h"
#include "BaseGS.h"

namespace Bitz
{
	namespace GFX
	{
		namespace GraphicsStates
		{
			class GS2D : public BaseGS
			{
			public:
				GS2D();
				~GS2D();
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