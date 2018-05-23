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
				std::vector<Effects::Light_Ptr> Lights;
				void AddLight(Effects::Light_Ptr);
				void RemoveLight(Effects::Light_Ptr);
			private:
				Camera_Ptr _ActiveCamera;
			};
		}
	}
}
