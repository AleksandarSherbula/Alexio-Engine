#include "aio_pch.h"
#include "GLFW_Window.h"
#include "GLFW/glfw3.h"

namespace Alexio
{   
    Window::~Window()
    {

    }

    GLFW_Window::GLFW_Window(const std::string& title, uint32_t width, uint32_t height)
    {
        mTitle = title;
        mWidth = width;
        mHeight = height;

        Initialize();
    }

    GLFW_Window::~GLFW_Window()
    {
        glfwDestroyWindow(mHandle);
        glfwTerminate();
    }

    void GLFW_Window::Initialize()
    {
        AIO_ASSERT(glfwInit(), "Failed to initialize GLFW");

        mHandle = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), NULL, NULL);
        AIO_ASSERT(mHandle, "Failed to create a GLFW window");

        glfwMakeContextCurrent(mHandle);
    }

    void GLFW_Window::Update()
    {
        glfwSwapBuffers(mHandle);
        glfwPollEvents();
    }   
}


