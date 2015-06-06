#pragma once
#include "../Common.h"
#include "../Core.h"
#include "../Graphics/Window.h"

namespace Bitz
{
	class Core;

	namespace GameLogic
	{
		class GameCore
		{
			friend class Bitz::Core;
		public:
			/// <summary>
			/// Starts the GameCore which in turn boots up the engine this is consumes the calling thread
			/// </summary>
			void Run(GFX::Window * initialWindow);

			virtual ~GameCore() {};

		protected:
			GameCore(std::string gameName);			
			/// <summary>
			/// This is executed once every game update cycle
			/// </summary>
			/// <returns>False is an error occures else true</returns>
			virtual bool Update() = 0;
			/// <summary>
			/// This is executed once every draw cycle and should be used for rendering specific logic only
			/// </summary>
			/// <returns>False is an error occures else true</returns>
			virtual bool Draw() = 0;
			/// <summary>
			/// This function is called once load content is complete to intialise the
			/// gamecore before the update and draw loops begin
			/// </summary>
			/// <returns>False is an error occures else true</returns>
			virtual bool Init() = 0;
			/// <summary>
			/// This function is called once for loading the content required by the game
			/// </summary>
			/// <returns>False is an error occures else true</returns>
			virtual bool LoadContent() = 0;
			/// <summary>
			/// This function is called once during the shutdown of the game to unload content
			/// </summary>
			/// <returns>False is an error occures else true</returns>
			virtual bool UnloadContent() = 0;
			/// <summary>
			/// This function is called when the core is attempting to exit the game
			/// </summary>
			/// <returns>False is an error occures else true</returns>
			virtual bool OnExit() = 0;

		private:
			/// <summary>
			/// The name of the game for logs
			/// </summary>
			std::string _GameName;
		};
	}
}
