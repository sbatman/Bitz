#include "../Common.h"
#include "Keyboard.h"

namespace Bitz
{
	namespace Input
	{
		enum Limits
		{
			KeyCount = 256
		};

		bool Keyboard::_CurrentKeyStates[Limits::KeyCount];
		bool Keyboard::_PreviousKeyStates[Limits::KeyCount];

		bool Keyboard::WasKeyReleased(KEY key)
		{
			if (key >= Limits::KeyCount) throw std::out_of_range("Key value out of range");
			return !_CurrentKeyStates[key] && _PreviousKeyStates[key];
		}
		bool Keyboard::WasKeyPressed(KEY key)
		{
			if (key >= Limits::KeyCount) throw std::out_of_range("Key value out of range");
			return _CurrentKeyStates[key] && !_PreviousKeyStates[key];
		}
		bool Keyboard::IsKeyDown(KEY key)
		{
			if (key >= Limits::KeyCount) throw std::out_of_range("Key value out of range");
			return _CurrentKeyStates[key];
		}

		void Keyboard::Init()
		{
			for (int x = 0; x < Limits::KeyCount; x++)
			{
				_CurrentKeyStates[x] = false;
				_PreviousKeyStates[x] = false;
			}
		}
		void Keyboard::StaticDispose()
		{
		}
		void Keyboard::Update()
		{
			for (int x = 0; x < Limits::KeyCount; x++)
			{
				_PreviousKeyStates[x] = _CurrentKeyStates[x];
			}
		}

#ifdef WIN32
		void Keyboard::HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
			case WM_KEYDOWN:
			{
				if (lParam & 1 << 30) return;
				_CurrentKeyStates[wParam] = true;
			}
			break;

			case WM_KEYUP:
			{
				_CurrentKeyStates[wParam] = false;
			}
			break;
			}
		}
#endif
	}
}