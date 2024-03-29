#pragma once

#include "Window/Window.h"

#if defined(AIO_API_DX11)
namespace Alexio
{
	class Win32_Window : public Window
	{
	public:
		Win32_Window(const std::string& title, uint32_t width, uint32_t height, const EventCallbackFn& eventCallback);
		~Win32_Window();
		void Initialize() override;
		void PollEvents() override;
		Vector2 GetMousePositionFromWindowAPI() override;

		inline void* GetHandle() const override { return mHandle; }
		void SetFullScreen(bool fullscreen) override;
	private:
		HWND mHandle;
		HINSTANCE m_hInstance;
		const WCHAR* mWindowClass;
	};
	
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::string ResultInfo(HRESULT hr);
}
#endif