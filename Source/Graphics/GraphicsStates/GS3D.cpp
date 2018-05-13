#include "../../Common.h"
#include "GS3D.h"
#include "../Shaders/Stock3D.h"
#include "../Shaders/ShaderService.h"

namespace Bitz
{
	namespace GFX
	{
		GraphicsStates::GS3D::GS3D()
		{
			_ActiveCamera = std::make_shared<Camera>();
			_ActiveCamera->SetMode(Camera::CameraMode::PERSPECTIVE);
			_CurrentBlendState = BlendStates::ALPHA;
			_StockShader = Shaders::ShaderService::CreateShader<Shaders::Stock3D>();
		}

		void GraphicsStates::GS3D::EnterState()
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			switch (_CurrentBlendState)
			{
			case BlendStates::NONE:
				break;
			case BlendStates::ALPHA:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BlendStates::ADDATIVE:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			}
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}

		void GraphicsStates::GS3D::ExitState()
		{
			if (_CurrentBlendState != BlendStates::NONE)glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
		}

		Camera_Ptr GraphicsStates::GS3D::GetActiveCamera() const
		{
			return _ActiveCamera;
		}

		bool GraphicsStates::GS3D::IsNormalsEnabled() const
		{
			return true;
		}
	}
}
