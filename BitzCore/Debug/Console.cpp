#include "../Common.h"
#include "Console.h"

namespace Bitz
{
	void Debug::Console::InitConsole()
	{
#ifdef WIN32
		AllocConsole();
		FILE* Console_Cout;
		FILE* Console_Cin;
		freopen_s(&Console_Cin, "CONIN$", "r", stdin);
		freopen_s(&Console_Cout, "CONOUT$", "w", stdout);
		freopen_s(&Console_Cout, "CONOUT$", "w", stderr);
#endif
	}
}