#pragma once
#include "../../Common.h"
#include "GraphicsState.h"
#include "../Effects/Light.h"

namespace Bitz
{
	namespace GFX
	{
		namespace GraphicsStates
		{
			class GS3D : public GraphicsState
			{
			public:
				GS3D();
				void EnterState() override;
				void ExitState() override;
				bool IsNormalsEnabled() const override;
				Camera_Ptr GetActiveCamera() const override;
				Light::Light_Ptr CurrentLight;
			private:
				Camera_Ptr _ActiveCamera;

			};
		}
	}
}
