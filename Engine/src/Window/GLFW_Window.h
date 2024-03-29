#pragma once

#include "Window/Window.h"

#if defined(AIO_API_OPENGL)
namespace Alexio
{
	class GLFW_Window : public Window
	{
	public:
		GLFW_Window(const std::string& title, uint32_t width, uint32_t height, const EventCallbackFn& eventCallback);
		~GLFW_Window();
		void Initialize();
		void PollEvents() override;
		Vector2 GetMousePositionFromWindowAPI() override;

		inline void* GetHandle() const override { return mHandle; }
		void SetFullScreen(bool fullscreen) override;
	private:
		void EventProcess();
	private:
		GLFWwindow* mHandle;
		GLFWmonitor* mMonitor;
		int32_t mPosX;
		int32_t mPosY;
	};
}
#endif