#include "Stock2D.h"

namespace Bitz
{
	namespace GFX
	{
		namespace Shaders
		{
			Stock2D::Stock2D()
			{
				std::string vertexShader = "";
				std::string fragmentShader = "";
				this->SetGLSL(vertexShader, fragmentShader);
			}
		}
	}
}
