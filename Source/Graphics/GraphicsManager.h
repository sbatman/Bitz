#pragma once

#include "Window.h"
#include "Drawables\IDrawable.h"
#include "Camera.h"
#include "GraphicsStates/BaseGS.h"
#include "RenderEngine.h"
#include "../Time/Timer.h"

namespace Bitz
{
	using namespace Math;

	class Core;

	namespace PlatformSpecific
	{
		namespace Windows { namespace GFX { class Window; } }
		namespace Android { namespace GFX { class Window; } }
	}

	namespace GFX
	{
		class Window;

		/// <summary>
		/// The graphics manager acts as an interface to the underlying drawing technology
		/// Handleing tasks such as screen clearing and sackbuffer swapping
		/// </summary>
		class GraphicsManager
		{
			friend class Bitz::GFX::Window;
			friend class Bitz::Core;
			friend class Bitz::PlatformSpecific::Windows::GFX::Window;
			friend class Bitz::PlatformSpecific::Android::GFX::Window;

		public:
			/// <summary>
			/// Sets the colour that will be used to clear the backbuffer
			/// </summary>
			/// <param name='newbufferClearColour'>The new colour to use when clearning the back buffer</param>
			static void SetBufferClearColour(const Vector3F newbufferClearColour);
			/// <summary>
			/// Sets the current screen size
			/// </summary>
			/// <param name='newbufferClearColour'>The size to change to window to match</param>
			static void SetScreenSize(Vector2I newSize);
			/// <summary>
			/// Gets the colour that will be used to clear the backbuffer
			/// </summary>
			/// <returns>The colour used to cleat the backbuffer</returns>
			static Vector3F GetBufferClearColour();
			/// <summary>
			/// Returns the last frame time in MS
			/// </summary>
			/// <returns>The last frame time in MS</returns>
			static double_t GetLastFrameTimeMS();
			/// <summary>
			/// Returns the current active camera
			/// </summary>
			/// <returns>A pointer to the camera currently active in the graphics manager</returns>
			static Camera * GetCurrentCamera();
			/// <summary>
			/// Gets the number of frames completed by the graphics manager
			/// </summary>
			/// <returns>The count of frames completed</returns>
			static uint64_t GetFrameCount();
			/// <summary>
			/// Gets the currnet size of the rendering window
			/// </summary>
			/// <returns>The count of frames completed</returns>
			static Vector2I GetScreenSize();
	
			/// <summary>
			/// Populates the VBO renderlist with this IDrawable
			/// </summary>
			/// <param name='idrawable'>A pointer to the the drawble to render</param>
			static void Render(std::shared_ptr<Drawables::IDrawable> idrawable);

			/// <summary>
			/// Begins a render pass causing the graphics manager to clear the screen and render screen contents to the buffer
			/// </summary>
			/// <param name='graphicsState'>The graphics state to use for this pass</param>
			static void BeginRender(GraphicsStates::BaseGS * graphicsState);
			/// <summary>
			/// Ends the render pass causing the graphics manager to present the current data to the screen
			/// </summary>
			static void EndRender();
			/// <summary>
			/// Forcefully clears the backbuffer of the rendering window
			/// </summary>
			static void Clear();
			/// <summary>
			/// Sets the window used for rendering
			/// </summary>
			/// <param name='graphicsState'>The window to use for rendering</param>
			static void SetActiveWindow(Window * window);
			/// <summary>
			/// Gets the currnet active render engine
			/// </summary>
			/// <returns>A pointer to the active render engine</returns>
			static RenderEngine * GetActiveRenderEngine();

		protected:
			/// <summary>
			/// Initalises the Graphics manager, making the rendering context current and creating resources
			/// </summary>
			/// <param name='window'>The window to be used by the rendering framework for output.</param>
			static void Init(Window * window);
			/// <summary>
			/// Standard update function for the graphics manager to allow it to perform required operations on the window
			/// </summary>
			static void Update();
			/// <summary>
			/// Disposes of all internal resources
			/// </summary>
			static void StaticDispose();
			/// <summary>
			/// Internal call for preparing the graphics manager for a render pass
			/// </summary>
			static void PreRender();
			/// <summary>
			/// Internal call to finalise and push draw instructions to the graphics card
			/// </summary>
			static void PostRender();

		private:
			/// <summary>
			/// The color that the backbuffer will be cleared
			/// </summary>
			static Vector3F _BufferClearColour;
			/// <summary>
			/// The current timer used for monitoring frame times
			/// </summary>
			static Time::Timer * _FrameTimer;
			/// <summary>
			/// A boolean representing whether the init has been run
			/// </summary>
			static bool _HasInit;
			/// <summary>
			/// A boolean representing whether we are currently between a render begin and end
			/// </summary>
			static bool _InRenderScope;

			/// <summary>
			/// When enabled it causes the backbuffer clear colour to change every frame between the specified colour and _Debug_BackbufferColourAlt
			/// </summary>
			static bool _Debug_EnableBackbufferColourAlt;
			/// <summary>
			/// Alternate colour used when _Debug_EnableBackbufferColourAlt is active
			/// <see cref="_Debug_EnableBackbufferColourAlt"/>
			/// </summary>
			static Vector3F _Debug_BackbufferColourAlt;

			/// <summary>
			/// The number of frames rendered by the render system since init
			/// </summary>
			static uint64_t _FrameNumber;
			/// <summary>
			/// The time taken to complete the last frame render
			/// </summary>
			static double_t _LastFrameTime;

			/// <summary>
			/// The current graphics state
			/// </summary>
			static GraphicsStates::BaseGS * _CurrentGraphicsState;
			/// <summary>
			/// The last active camera saved for some optimisations like projection matix baking
			/// </summary>
			static Camera * _LastActiveCamera;
			/// <summary>
			/// A pointer to the active render engine
			/// </summary>
			static RenderEngine * _ActiveRenderEngine;
			/// <summary>
			/// A pointer to the active window
			/// </summary>
			static Window * _ActiveWindow;
		};
	}
}