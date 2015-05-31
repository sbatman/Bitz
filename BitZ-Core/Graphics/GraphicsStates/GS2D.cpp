#include "../../Common.h"
#include "GS2D.h"

namespace Bitz
{
	namespace GFX
	{
		GraphicsStates::GS2D::GS2D()
		{
			_ActiveCamera = new Camera();
			_ActiveCamera->SetMode(Camera::CameraMode::Ortho);
			_CurrentBlendState = BlendStates::ALPHA;
		}

		GraphicsStates::GS2D::~GS2D()
		{
			delete _ActiveCamera;
		}

		void GraphicsStates::GS2D::EnterState()
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_CULL_FACE);
			glEnable(GL_NORMALIZE);
			switch (_CurrentBlendState)
			{
			case BlendStates::NONE:
				break;
			case BlendStates::ALPHA:
				glEnable(GL_BLEND);
#ifdef __ANDROID__
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#elif WIN32
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
#endif

				break;
			case BlendStates::ADDATIVE:
				glEnable(GL_BLEND);
#ifdef __ANDROID__
#elif WIN32
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
#endif

				break;
			}
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
		}

		void GraphicsStates::GS2D::ExitState()
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_CULL_FACE);
			glDisable(GL_NORMALIZE);
			if (_CurrentBlendState != BlendStates::NONE)glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_STENCIL_TEST);
		}

		GFX::Camera* GraphicsStates::GS2D::GetActiveCamera() const
		{
			return _ActiveCamera;
		}

		bool GraphicsStates::GS2D::IsNormalsEnabled()
		{
			return false;
		}
	}
}