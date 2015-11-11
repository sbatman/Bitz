#include "../../Common.h"
#include "GS3D.h"

namespace Bitz
{
	namespace GFX
	{
		GraphicsStates::GS3D::GS3D()
		{
			_ActiveCamera = new Camera();
			_ActiveCamera->SetMode(Camera::CameraMode::Perspective);
			_CurrentBlendState = BlendStates::ALPHA;
		}

		GraphicsStates::GS3D::~GS3D()
		{
			delete _ActiveCamera;
		}

		void GraphicsStates::GS3D::EnterState()
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			glDisable(GL_LIGHTING);
			//glEnable(GL_LIGHTING);
			glEnable(GL_NORMALIZE);
			switch (_CurrentBlendState)
			{
			case BlendStates::NONE:
				break;
			case BlendStates::ALPHA:
				glEnable(GL_BLEND);
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
				break;
			case BlendStates::ADDATIVE:
				glEnable(GL_BLEND);
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			}
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			//glEnable(GL_STENCIL_TEST);
		}

		void GraphicsStates::GS3D::ExitState()
		{
			glEnable(GL_LIGHTING);
			//glDisable(GL_LIGHTING);
			glDisable(GL_NORMALIZE);
			if (_CurrentBlendState != BlendStates::NONE)glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			//glDisable(GL_STENCIL_TEST);
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