#include "../Common.h"
#include "GameCore.h"
#include "../Core.h"
#include "Graphics/Window.h"

namespace Bitz
{
	namespace GameLogic
	{
		GameCore::GameCore(std::string gameName)
		{
			_GameName = gameName;
		}

		void GameCore::Run(GFX::Window * initialWindow)
		{
			Core::Init();

			initialWindow->SetQuitFunction(Core::Stop);

			Core::Run(this);
		}
	}
}