#include "../../Common.h"
#include "GS2D.h"
#include "../Shaders/Stock2D.h"

namespace Bitz
{
	namespace GFX
	{
		GraphicsStates::GS2D::GS2D()
		{
			_ActiveCamera = std::make_shared<Camera>();
			_ActiveCamera->SetMode(Camera::CameraMode::ORTHO);
			_CurrentBlendState = BlendStates::ALPHA;
			_StockShader = std::static_pointer_cast<Shaders::Shader>(std::make_shared<Shaders::Stock2D>());
		}

		GraphicsStates::GS2D::~GS2D()
		{
			_ActiveCamera = nullptr;
			_StockShader = nullptr;
		}

		void GraphicsStates::GS2D::EnterState()
		{
			glDisable(GL_CULL_FACE);
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
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
		}

		void GraphicsStates::GS2D::ExitState()
		{
			glEnable(GL_CULL_FACE);
			if (_CurrentBlendState != BlendStates::NONE)glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}

		GFX::Camera_Ptr GraphicsStates::GS2D::GetActiveCamera() const
		{
			return _ActiveCamera;
		}

		bool GraphicsStates::GS2D::IsNormalsEnabled() const
		{
			return false;
		}
	}
}