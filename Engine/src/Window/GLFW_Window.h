#pragma once

#include "Window/Window.h"

#if defined(AIO_API_OPENGL)
namespace Alexio
{
	class GLFW_Window : public Window
	{
	public:
		GLFW_Window(const std::string& title, uint32_t width, uint32_t height);
		~GLFW_Window();
		void Initialize();
		void PollEvents() override;
		glm::vec2 GetMousePositionFromWindowAPI() override;

		inline void* GetHandle() const override { return mHandle; }
		inline void SetEventCallback(const EventCallbackFn& callback) override { mData.eventCallback = callback; }
		void SetFullScreen(bool fullscreen) override;
	private:
		void EventProcess();
	private:
		GLFWwindow* mHandle;
		GLFWmonitor* mMonitor;
		int32_t mPosX;
		int32_t mPosY;

		struct WindowDataFromCallback
		{
			std::string title;
			uint32_t width, height;

			EventCallbackFn eventCallback;
		};

		WindowDataFromCallback mData;
	};
}
#endif