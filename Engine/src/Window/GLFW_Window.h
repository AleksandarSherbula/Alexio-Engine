#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include "Window.h"

class GLFWwindow;

namespace Alexio
{
	class GLFW_Window : public Window
	{
	public:
		GLFW_Window(const std::string& title, uint32_t width, uint32_t height);
		~GLFW_Window();
		void Initialize();
		void Update() override;

		inline void* GetHandle() override { return mHandle; }
	private:
		GLFWwindow* mHandle;
	};
}

#endif // !GLFW_WINDOW_H



