#include "../../Common.h"
#include "GS3D.h"
#include "../Shaders/Stock3D.h"

namespace Bitz
{
	namespace GFX
	{
		GraphicsStates::GS3D::GS3D()
		{
			_ActiveCamera = new Camera();
			_ActiveCamera->SetMode(Camera::CameraMode::Perspective);
			_CurrentBlendState = BlendStates::ALPHA;
			_StockShader = Shaders::Shader_Ptr(new Shaders::Stock3D());
		}

		GraphicsStates::GS3D::~GS3D()
		{
			delete _ActiveCamera;
		}

		void GraphicsStates::GS3D::EnterState()
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_NORMALIZE);
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
		//	glDepthFunc(GL_LEQUAL);
		}

		void GraphicsStates::GS3D::ExitState()
		{
			glDisable(GL_NORMALIZE);
			if (_CurrentBlendState != BlendStates::NONE)glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
		}

		Camera* GraphicsStates::GS3D::GetActiveCamera() const
		{
			return _ActiveCamera;
		}

		bool GraphicsStates::GS3D::IsNormalsEnabled() const
		{
			return true;
		}
	}
}