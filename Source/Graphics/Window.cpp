#include "Window.h"
#include "GraphicsManager.h"
#include "../Input/Keyboard.h"


namespace Bitz
{

	namespace GFX
	{
		Window::Window(std::wstring title, uint32_t width, uint32_t height, HINSTANCE hInstance)
		{
			_WindowSize = Vector2I(width, height);
			// register window class
			_WC.style = CS_OWNDC;
			_WC.lpfnWndProc = &GFX::Window::WndProc;
			_WC.cbClsExtra = 0;
			_WC.cbWndExtra = 0;
			_WC.hInstance = hInstance;
			_WC.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
			_WC.hCursor = LoadCursor(nullptr, IDC_ARROW);
			_WC.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
			_WC.lpszMenuName = nullptr;
			_WC.lpszClassName = title.c_str();
			RegisterClass(&_WC);

			// create main window
			_HWnd = CreateWindow(title.c_str(), title.c_str(), WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, _WindowSize.X, _WindowSize.Y, NULL, NULL, hInstance, NULL);
			ApplyWindowSizeChange();
			_GlContext = new Bitz::GFX::GLContext();
			_GlContext->Init(_HWnd);

			Bitz::GFX::GraphicsManager::Init(this);
		}

		Window::~Window()
		{
			delete _GlContext;
			_GlContext = nullptr;
		}

		void Window::SetQuitFunction(void(function)(void))
		{
			OnQuitEvent = function;
		}

		Bitz::Math::Vector2I Window::GetWindowSize() const
		{
			return _WindowSize;
		}

		void Window::SetWindowSize(const Vector2I newSize)
		{
			_WindowSize = newSize;
			ApplyWindowSizeChange();
		}

		void Window::Update()
		{
			MSG msg;
			// check for messages
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// handle or dispatch messages
				if (msg.message == WM_QUIT)
				{
					if (OnQuitEvent != nullptr)OnQuitEvent();
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}

		GLContext * Window::GetGLContext() const
		{
			return _GlContext;
		}

		void Window::ApplyWindowSizeChange()
		{
			DWORD dwStyle = static_cast<DWORD>(GetWindowLongPtr(_HWnd, GWL_STYLE));
			DWORD dwExStyle = static_cast<DWORD>(GetWindowLongPtr(_HWnd, GWL_EXSTYLE));
			HMENU menu = GetMenu(_HWnd);
			RECT windowSize = { 0, 0, _WindowSize.X, _WindowSize.Y };;
			AdjustWindowRectEx(&windowSize, dwStyle, menu != nullptr, dwExStyle);
			SetWindowPos(_HWnd, nullptr, 0, 0, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, SWP_NOZORDER | SWP_NOMOVE);
		}

		LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
			case WM_CREATE:
				return 0;
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
			case WM_DESTROY:
				return 0;
			case WM_KEYDOWN:
				switch (wParam)
				{
				case VK_ESCAPE:
					PostQuitMessage(0);
					return 0;
				default:
					Bitz::Input::Keyboard::HandleWindowMessage(hWnd, message, wParam, lParam);
					break;
				}
				return 0;
			case WM_KEYUP:
				Bitz::Input::Keyboard::HandleWindowMessage(hWnd, message, wParam, lParam);
				break;
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
				//Bitz::Input::Mouse::HandleWindowMessage(hWnd, message, wParam, lParam);
				return 0;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		}


	}
}