#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H

#include "Window/Window.h"

namespace Alexio
{
	class Win32_Window : public Window
	{
	public:
		Win32_Window(const std::string& title, uint32_t width, uint32_t height);
		~Win32_Window();
		void Initialize() override;
		void Update() override;

		void PollEvents() override;

		inline void* GetHandle() override { return mHandle; }
		void SetEventCallback(const EventCallbackFn& callback) override;
	private:
		HWND mHandle;
		HINSTANCE m_hInstance;
		const WCHAR* mWindowClass;
	};
	
	LRESULT CALLBACK WindowsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK KeyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MouseProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::string ResultInfo(HRESULT hr);
}

#endif // !WIN32_WINDOW_H