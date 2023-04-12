#include "aio_pch.h"

#if defined(AIO_API_DX11)
#include "Win32_Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/DX11/DX11_Backend.h"

#include <imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Alexio
{
	Win32_Window::Win32_Window(const std::string& title, uint32_t width, uint32_t height, const EventCallbackFn& eventCallback)
	{
		mTitle = title;
		mCallbackData.width = width;
		mCallbackData.height = height;
		mCallbackData.eventCallback = eventCallback;

		mWindowClass = L"Win32 Class";
		m_hInstance = GetModuleHandle(nullptr);

		mIsFullScreen = false;

		Initialize();
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
		windowRect.right = windowRect.left + mCallbackData.width;
		windowRect.bottom = windowRect.top + mCallbackData.height;
		if (!AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE))
			std::cout << "Failed to adjust window" << std::endl;

		mHandle = CreateWindowEx(0, mWindowClass, StringToWide(mTitle).c_str(), WS_OVERLAPPEDWINDOW,
			windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			NULL, NULL, m_hInstance, &mCallbackData);
		
		AIO_ASSERT(mHandle, "Failed to create a Window: {0}", ResultInfo(GetLastError()));
		
		ShowWindow(mHandle, SW_MAXIMIZE);
		SetForegroundWindow(mHandle);
		SetFocus(mHandle);
	}

	void Win32_Window::SetFullScreen(bool fullscreen)
	{
		if (IsFullScreen() == fullscreen)
			return;

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

		mIsFullScreen = fullscreen;
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

	glm::vec2 Win32_Window::GetMousePositionFromWindowAPI()
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(mHandle, &mousePos);
		return glm::vec2(mousePos.x, mousePos.y);
	}

	LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		// WINDOW EVENTS
		switch (uMsg)
		{
		case WM_CREATE:
		{
			CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			WindowDataFromCallback* data = reinterpret_cast<WindowDataFromCallback*>(createStruct->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
			return 0;
		}
		case WM_SIZE:
		{			
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);

			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			data->width = width;
			data->height = height;
			
			WindowResizeEvent event(width, height);
			data->eventCallback(event);
			return 0;
		}
		case WM_CLOSE:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			WindowCloseEvent event;
			data->eventCallback(event);
			return 0;
		}

		// KEY EVENTS
		case WM_SYSKEYDOWN:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

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
			data->eventCallback(event);
			Input::UpdateKeyState(keycode, true);
			return 0;
		}
		case WM_SYSKEYUP:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

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
			data->eventCallback(event);
			Input::UpdateKeyState(keycode, false);
			return 0;
		}
		case WM_KEYDOWN:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			KeyPressedEvent event(wParam, LOWORD(lParam));
			data->eventCallback(event);

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
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			KeyReleasedEvent event(wParam);
			data->eventCallback(event);
			

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
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			MouseButtonPressedEvent event(MK_LBUTTON);
			data->eventCallback(event);
			
			Input::UpdateMouseState(L_BUTTON, true);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);			

			MouseButtonPressedEvent event(MK_RBUTTON);
			data->eventCallback(event);
			
			Input::UpdateMouseState(R_BUTTON, true);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			MouseButtonPressedEvent event(MK_MBUTTON);
			data->eventCallback(event);
			Input::UpdateMouseState(M_BUTTON, true);
			return 0;
		}
		case WM_XBUTTONDOWN:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			int mouseButton = (wParam & MK_XBUTTON1) | (wParam & MK_XBUTTON2);
			MouseButtonPressedEvent event(mouseButton);
			data->eventCallback(event);
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
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			MouseButtonReleasedEvent event(MK_LBUTTON);
			data->eventCallback(event);
			Input::UpdateMouseState(L_BUTTON, false);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			MouseButtonReleasedEvent event(MK_RBUTTON);
			data->eventCallback(event);
			Input::UpdateMouseState(R_BUTTON, false);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			MouseButtonReleasedEvent event(MK_MBUTTON);
			data->eventCallback(event);
			Input::UpdateMouseState(M_BUTTON, false);
			return 0;
		}
		case WM_XBUTTONUP:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			UINT xbuttoncode = wParam;
			if (wParam == 131072)		xbuttoncode = (wParam | 0x0040) & 0x0040;
			else if (wParam == 65536)	xbuttoncode = (wParam | 0x0020) & 0x0020;
			MouseButtonReleasedEvent event(xbuttoncode);
			data->eventCallback(event);
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
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			// I have no mouse to test the x-axis scrolling so for now it's set to 0.
			MouseScrolledEvent event(0, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			data->eventCallback(event);
			return 0;
		}

		// MOUSE MOVE EVENTS
		case WM_MOUSEMOVE:
		{
			WindowDataFromCallback* data = (WindowDataFromCallback*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			MouseMovedEvent event((float)LOWORD(lParam), (float)HIWORD(lParam));
			data->eventCallback(event);
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
#endif