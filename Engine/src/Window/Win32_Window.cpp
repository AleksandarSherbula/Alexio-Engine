#include "aio_pch.h"
#include "Win32_Window.h"
#include "Alexio/Engine.h"
#include "Alexio/Input.h"

#include "Alexio/Renderer.h"
#include "Renderer/DX11/DX11_Renderer.h"

#include "Events/AppEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Alexio
{
	static EventCallbackFn ecFn;

	Win32_Window::Win32_Window(const std::string& title, uint32_t width, uint32_t height)
	{
		mWidth = width;
		mHeight = height;
		mTitle = title;
		mWindowClass = L"Win32 Class";
		m_hInstance = GetModuleHandle(nullptr);
	}

	Win32_Window::~Win32_Window()
	{
		DestroyWindow(mHandle);
		UnregisterClass(mWindowClass, m_hInstance);
	}

	void Win32_Window::Initialize()
	{
		WNDCLASSEX wc;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = mWindowClass;
		wc.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&wc);

		RECT windowRect;
		
		windowRect.left = 50;
		windowRect.top = 50;
		windowRect.right = windowRect.left + mWidth;
		windowRect.bottom = windowRect.top + mHeight;
		if (!AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE))
			std::cout << "Failed to adjust window" << std::endl;

		DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

		mHandle = CreateWindowEx(0, mWindowClass, StringToWide(mTitle).c_str(), WS_OVERLAPPEDWINDOW,
			windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			NULL, NULL, m_hInstance, NULL);
		
		AIO_ASSERT(mHandle, "Failed to create a Window: {0}", ResultInfo(GetLastError()));

		ShowWindow(mHandle, SW_SHOWDEFAULT);
		SetForegroundWindow(mHandle);
		SetFocus(mHandle);
	}

	void Win32_Window::SetEventCallback(const EventCallbackFn& callback)
	{		
		ecFn = callback;
		Initialize();
	}

	void Win32_Window::SetFullScreen(bool fullscreen)
	{
		static RECT windowRect = {};

		DWORD style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
		SetWindowLong(mHandle, GWL_STYLE, style);

		if (fullscreen)
		{
			MONITORINFO mi = { sizeof(mi) };
			if (GetMonitorInfo(MonitorFromWindow(mHandle,
					MONITOR_DEFAULTTOPRIMARY), &mi))
			{
				GetWindowRect(mHandle, &windowRect);
				SetWindowPos(mHandle, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_SHOWWINDOW);
			}
		}
		else 
		{
			SetWindowPos(mHandle, NULL,
				windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
				SWP_SHOWWINDOW);
		}
	}

	void Win32_Window::PollEvents()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));	

		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		
	}

	LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		// WINDOW EVENTS
		switch (uMsg)
		{
		case WM_SIZE:
		{			
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);

			WindowResizeEvent event(width, height);
			ecFn(event);
			return 0;
		}
		case WM_CLOSE:
		{
			WindowCloseEvent event;
			ecFn(event);
			return 0;
		}


		// KEY EVENTS
		case WM_SYSKEYDOWN:
		{
			int32_t keycode;
			int32_t isRight = (lParam >> 24) & 0x01;

			switch (wParam)
			{
			case 18: keycode = (isRight) ? Key::R_ALT : Key::L_ALT; break;
			case 121: keycode = Key::F10; break;
			default:
				keycode = Input::mapKeys[wParam];
			}
			KeyPressedEvent event(wParam, LOWORD(lParam));
			ecFn(event);
			Input::UpdateKeyState(keycode, true);
			return 0;
		}
		case WM_SYSKEYUP:
		{
			int32_t keycode;
			int32_t isRight = (lParam >> 24) & 0x01;

			switch (wParam)
			{
			case 18: keycode = (isRight) ? Key::R_ALT : Key::L_ALT; break;
			case 121: keycode = Key::F10; break;
			default:
				keycode = Input::mapKeys[wParam];
			}
			KeyPressedEvent event(wParam, LOWORD(lParam));
			ecFn(event);
			Input::UpdateKeyState(keycode, false);
			return 0;
		}
		case WM_KEYDOWN:
		{
			KeyPressedEvent event(wParam, LOWORD(lParam));
			ecFn(event);

			int32_t keycode;
			bool isRight;

			switch (wParam)
			{
			case 16:
			{
				isRight = (lParam >> 20) & 0x01;
				keycode = (isRight) ? Key::R_SHIFT : Key::L_SHIFT; break;
			}
			case 17:
			{
				isRight = (lParam >> 24) & 0x01;
				keycode = (isRight) ? Key::R_CTRL : Key::L_CTRL; break;
			}
			default:
				keycode = Input::mapKeys[wParam];
			}

			Input::UpdateKeyState(keycode, true);
			return 0;
		}
		case WM_KEYUP:
		{
			KeyReleasedEvent event(wParam);
			ecFn(event);

			int32_t keycode;
			int32_t isRight;

			switch (wParam)
			{
			case 16:
			{
				isRight = (lParam >> 20) & 0x01;
				keycode = (isRight) ? Key::R_SHIFT : Key::L_SHIFT; break;
			}
			case 17:
			{
				isRight = (lParam >> 24) & 0x01;
				keycode = (isRight) ? Key::R_CTRL : Key::L_CTRL; break;
			}
			default:
				keycode = Input::mapKeys[wParam];
			}

			Input::UpdateKeyState(keycode, false);
			return 0;
		}

		// MOUSE PRESS EVENTS
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent event(MK_LBUTTON);
			ecFn(event);
			Input::UpdateMouseState(Alexio::L_BUTTON, true);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent event(MK_RBUTTON);
			ecFn(event);
			Input::UpdateMouseState(Alexio::R_BUTTON, true);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent event(MK_MBUTTON);
			ecFn(event);
			Input::UpdateMouseState(Alexio::M_BUTTON, true);
			return 0;
		}
		case WM_XBUTTONDOWN:
		{
			int mouseButton = (wParam & MK_XBUTTON1) | (wParam & MK_XBUTTON2);
			MouseButtonPressedEvent event(mouseButton);
			ecFn(event);
			if (mouseButton == 32)
				mouseButton = X_BUTTON1;
			else if (mouseButton == 64)
				mouseButton = X_BUTTON2;
			Input::UpdateMouseState(mouseButton, true);
			return 0;
		}

		// MOUSE RELEASE EVENTS
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent event(MK_LBUTTON);
			ecFn(event);
			Input::UpdateMouseState(Alexio::L_BUTTON, false);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent event(MK_RBUTTON);
			ecFn(event);
			Input::UpdateMouseState(Alexio::R_BUTTON, false);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent event(MK_MBUTTON);
			ecFn(event);
			Input::UpdateMouseState(Alexio::M_BUTTON, false);
			return 0;
		}
		case WM_XBUTTONUP:
		{
			UINT xbuttoncode = wParam;
			if (wParam == 131072)		xbuttoncode = (wParam | 0x0040) & 0x0040;
			else if (wParam == 65536)	xbuttoncode = (wParam | 0x0020) & 0x0020;
			MouseButtonReleasedEvent event(xbuttoncode);
			ecFn(event);
			if (xbuttoncode == 32)
				xbuttoncode = X_BUTTON1;
			else if (xbuttoncode == 64)
				xbuttoncode = X_BUTTON2;
			Input::UpdateMouseState(xbuttoncode, false);
			return 0;
		}

		// MOUSE SCROLL EVENTS
		case WM_MOUSEWHEEL:
		{
			// I have no mouse to test the x-axis scrolling so for now it's set to 0.
			MouseScrolledEvent event(0, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			ecFn(event);
			return 0;
		}

		// MOUSE MOVE EVENTS
		case WM_MOUSEMOVE:
		{
			MouseMovedEvent event((float)LOWORD(lParam), (float)HIWORD(lParam));
			ecFn(event);
			return 0;
		}

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		};
	}

	std::string ResultInfo(HRESULT hr)
	{
		_com_error error(hr);
		return WideToString(error.ErrorMessage());
	}
}