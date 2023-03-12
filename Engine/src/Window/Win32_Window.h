#pragma once

#include "Window/Window.h"

namespace Alexio
{
	class Win32_Window : public Window
	{
	public:
		Win32_Window(const std::string& title, uint32_t width, uint32_t height);
		~Win32_Window();
		void Initialize() override;
		void PollEvents() override;

		inline void* GetHandle() const override { return mHandle; }
		void SetEventCallback(const EventCallbackFn& callback) override;
		inline bool IsFullScreen() const override { return false; }
		void SetFullScreen(bool fullscreen) override;
	private:
		HWND mHandle;
		HINSTANCE m_hInstance;
		const WCHAR* mWindowClass;
	};
	
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::string ResultInfo(HRESULT hr);
}