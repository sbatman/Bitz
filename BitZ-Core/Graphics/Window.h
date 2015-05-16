#pragma once
#include "GLContext.h"

namespace Bitz
{
	namespace GFX
	{
		class Window
		{
		public:
			Window(std::string title, uint32_t width, uint32_t height);
			~Window();
			void Update();
			GLContext * GetGLContext() const;
			void SetQuitFunction(void * function);
			Vector2I GetWindowSize() const;
			void SetWindowSize(const Vector2I newSize);

		protected:
			GLContext * _GlContext = nullptr;
			void(*OnQuitEvent)() = nullptr;

			Vector2I _WindowSize;
			void ApplyWindowSizeChange();
		};
	}
}