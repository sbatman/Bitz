#pragma once
#include "../../Common.h"
#include "BaseGS.h"

namespace Bitz
{
	namespace GFX
	{
		namespace GraphicsStates
		{
			class GS3D : public BaseGS
			{
			public:
				GS3D();
				~GS3D();
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