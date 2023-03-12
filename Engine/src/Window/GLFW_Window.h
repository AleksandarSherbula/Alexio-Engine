#pragma once

#include "Window/Window.h"

namespace Alexio
{
	class GLFW_Window : public Window
	{
	public:
		GLFW_Window(const std::string& title, uint32_t width, uint32_t height);
		~GLFW_Window();
		void Initialize();
		void PollEvents() override;

		inline void* GetHandle() const override { return mHandle; }
		inline void SetEventCallback(const EventCallbackFn& callback) override { mData.eventCallback = callback; }
		inline bool IsFullScreen() const override { return glfwGetWindowMonitor(mHandle) != nullptr; }
		void SetFullScreen(bool fullscreen) override;
	private:
		void EventProcess();
	private:
		GLFWwindow* mHandle;
		GLFWmonitor* mMonitor;
		int32_t mPosX;
		int32_t mPosY;

		struct WindowData
		{
			std::string title;
			uint32_t width, height;

			EventCallbackFn eventCallback;
		};

		WindowData mData;
	};
}