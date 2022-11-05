#include "aio_pch.h"
#include "Win32_Window.h"

namespace Alexio
{
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

		RECT wr; //Window Rectangle
		wr.left = 50;
		wr.top = 50;
		wr.right = wr.left + mWidth;
		wr.bottom = wr.top + mHeight;
		if (!AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE))
			std::cout << "Failed to adjust window" << std::endl;

		mHandle = CreateWindowEx(0, mWindowClass, StringToWide(mTitle).c_str(), WS_OVERLAPPEDWINDOW,
			wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
			NULL, NULL, m_hInstance, NULL);

		if (!mHandle)
		{
			HRESULT hr = GetLastError();
			std::cout << "Failed to create a Window" << std::endl;
			std::cout << ResultInfo(hr) << std::endl;
		}

		ShowWindow(mHandle, SW_SHOW);
		SetForegroundWindow(mHandle);
		SetFocus(mHandle);
	}

	void Win32_Window::Update()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		if (PeekMessage(&msg, mHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_NULL)
		{
			if (!IsWindow(mHandle))
			{
				mHandle = nullptr;
				UnregisterClass(mWindowClass, m_hInstance);
				//return false;
			}
		}

		//return true;
	}

	LRESULT WindowsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	std::string ResultInfo(HRESULT hr)
	{
		_com_error error(hr);
		return WideToString(error.ErrorMessage());
	}
}