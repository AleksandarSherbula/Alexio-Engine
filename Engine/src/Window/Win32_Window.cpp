#include "aio_pch.h"
#include "Win32_Window.h"

#include "Events/AppEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Alexio
{
	std::function<void(Event&)> ecFn;

	Win32_Window::Win32_Window(const std::string& title, uint32_t width, uint32_t height)
	{
		mWidth = width;
		mHeight = height;
		mTitle = title;
		mWindowClass = L"Win32 Class";
		m_hInstance = GetModuleHandle(nullptr);

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
		wc.lpfnWndProc = WindowsProc;
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

		int ResX = GetSystemMetrics(SM_CXSCREEN);
		int ResY = GetSystemMetrics(SM_CYSCREEN);

		RECT wr; 
		wr.left = 50;
		wr.top = 50;
		wr.right = wr.left + mWidth;
		wr.bottom = wr.top + mHeight;
		if (!AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE))
			std::cout << "Failed to adjust window" << std::endl;

		mHandle = CreateWindowEx(0, mWindowClass, StringToWide(mTitle).c_str(), WS_OVERLAPPEDWINDOW,
			wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
			NULL, NULL, m_hInstance, NULL);
		
		AIO_ASSERT(mHandle, "Failed to create a Window: {0}", ResultInfo(GetLastError()));

		
	}

	void Win32_Window::SetEventCallback(const EventCallbackFn& callback)
	{		
		ecFn = callback;
		ShowWindow(mHandle, SW_SHOW);
		SetForegroundWindow(mHandle);
		SetFocus(mHandle);
	}

	void Win32_Window::Update()
	{
		ProcessEvents();
	}

	void Win32_Window::ProcessEvents()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));	

		if (PeekMessage(&msg, mHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		
	}

	LRESULT WindowsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
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
		default:
			return KeyProc(hwnd, uMsg, wParam, lParam);
		};
		
	}

	LRESULT KeyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
		{
			KeyPressedEvent event(wParam, LOWORD(lParam));
			ecFn(event);
			return 0;
		}
		case WM_KEYUP:
		{
			KeyReleasedEvent event(wParam);
			ecFn(event);
			return 0;
		}
		default:
			return MouseProc(hwnd, uMsg, wParam, lParam);
		};
		
	}

	LRESULT MouseProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{

		// MOUSE PRESS EVENTS
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent event(MK_LBUTTON);
			ecFn(event);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent event(MK_RBUTTON);
			ecFn(event);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent event(MK_MBUTTON);
			ecFn(event);
			return 0;
		}
		case WM_XBUTTONDOWN:
		{
			MouseButtonPressedEvent event((wParam & MK_XBUTTON1) | (wParam & MK_XBUTTON2));
			ecFn(event);
			return 0;
		}

		// MOUSE RELEASE EVENTS
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent event(MK_LBUTTON);
			ecFn(event);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent event(MK_RBUTTON);
			ecFn(event);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent event(MK_MBUTTON);
			ecFn(event);
			return 0;
		}
		case WM_XBUTTONUP:
		{
			UINT xbuttoncode = wParam;
			if (wParam == 131072)		xbuttoncode = (wParam | 0x0040) & 0x0040;
			else if (wParam == 65536)	xbuttoncode = (wParam | 0x0020) & 0x0020;

			MouseButtonReleasedEvent event(xbuttoncode);
			ecFn(event);
			return 0;
		}

		// MOUSE SCROLL EVENTS
		// TODO

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