#pragma once
#include "GLContext.h"

namespace Bitz
{
	namespace GFX
	{
		class Window
		{
		public:
			virtual ~Window() {};
			virtual void Update() = 0;
			virtual GLContext * GetGLContext() const = 0;
			virtual void SetQuitFunction(void (function)(void)) = 0;
			virtual Vector2I GetWindowSize() const = 0;
			virtual void SetWindowSize(const Vector2I newSize) = 0;

		protected:
			void(*OnQuitEvent)() = nullptr;

			Vector2I _WindowSize;
			virtual void ApplyWindowSizeChange() = 0;
		};
	}
}