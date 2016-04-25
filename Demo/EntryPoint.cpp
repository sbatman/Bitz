#include <Windows.h>
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t iCmdShow)
{
	Bitz::Debug::Console::InitConsole();

	Game * gameInstance = new Game();
	Bitz::GFX::Window * initalWindow = new Bitz::GFX::Window(L"TestApp", 1440, 900, hInstance);
	gameInstance->Run(initalWindow);
	delete gameInstance;
	return 0;
}