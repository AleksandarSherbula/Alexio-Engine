#pragma once

#include "Alexio/Window.h"

namespace Alexio
{
	class GLFW_Window : public Window
	{
	public:
		GLFW_Window(const std::string& title, uint32_t width, uint32_t height);
		~GLFW_Window();
		void Initialize();
		void PollEvents() override;

		inline void* GetHandle() override { return mHandle; }
		void SetEventCallback(const EventCallbackFn& callback) override { mData.eventCallback = callback; }
	private:
		void EventProcess();
	private:
		GLFWwindow* mHandle;

		struct WindowData
		{
			std::string title;
			uint32_t width, height;

			EventCallbackFn eventCallback;
		};

		WindowData mData;
	};
}