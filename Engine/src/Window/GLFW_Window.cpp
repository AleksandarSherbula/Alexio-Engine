#include "aio_pch.h"

#if defined(AIO_API_OPENGL)
#include "GLFW_Window.h"

#include "Renderer/Renderer.h"

namespace Alexio
{
    static bool sGLFW_Initialize = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        AIO_LOG_ERROR("GLFW Error {0}: {1}", error, description);
    }

    GLFW_Window::GLFW_Window(const std::string& title, uint32_t width, uint32_t height, const EventCallbackFn& eventCallback)
    {
        mTitle = title;
        mCallbackData.width = width;
        mCallbackData.height = height;
        mCallbackData.eventCallback = eventCallback;
        mIsFullScreen = false;

        Initialize();
    }

    GLFW_Window::~GLFW_Window()
    {
        glfwDestroyWindow(mHandle);
        glfwTerminate();
    }

    void GLFW_Window::Initialize()
    {
        if (!sGLFW_Initialize)
        {
            AIO_ASSERT(glfwInit(), "Failed to initialize GLFW");
            glfwSetErrorCallback(GLFWErrorCallback);
            sGLFW_Initialize = true;
        }
#ifdef AIO_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // AIO_DEBUG

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

        mHandle = glfwCreateWindow(mCallbackData.width, mCallbackData.height, mTitle.c_str(), NULL, NULL);
        AIO_ASSERT(mHandle, "Failed to create a GLFW window");

        glfwMakeContextCurrent(mHandle);
        glfwSetWindowUserPointer(mHandle, &mCallbackData);

        mMonitor = glfwGetPrimaryMonitor();

        EventProcess();
    }

    void GLFW_Window::PollEvents()
    {
        glfwPollEvents();
    }

    glm::vec2 GLFW_Window::GetMousePositionFromWindowAPI()
    {
        double x, y;
        glfwGetCursorPos(mHandle, &x, &y);

        return glm::vec2(x, y);
    }

    void GLFW_Window::SetFullScreen(bool fullscreen)
    {
        if (IsFullScreen() == fullscreen)
            return;

        if (fullscreen)
        {
            glfwGetWindowPos(mHandle, &mPosX, &mPosY);
            glfwGetWindowSize(mHandle, (int32_t*)&mCallbackData.width, (int32_t*)&mCallbackData.height);

            const GLFWvidmode* mode = glfwGetVideoMode(mMonitor);
            glfwSetWindowMonitor(mHandle, mMonitor, 0, 0, mode->width, mode->height, 0);
        }
        else
        {
            glfwSetWindowMonitor(mHandle, nullptr, mPosX, mPosY, mCallbackData.width, mCallbackData.height, (int32_t)Renderer::GetBackend()->IsVSync());
        }
        mIsFullScreen = fullscreen;
    }

    void GLFW_Window::EventProcess()
    {
        glfwSetWindowSizeCallback(mHandle, [](GLFWwindow* window, int width, int height)
            {
                WindowDataFromCallback& data = *(WindowDataFromCallback*)glfwGetWindowUserPointer(window);
                data.width = width;
                data.height = height;

                WindowResizeEvent event(width, height);
                data.eventCallback(event);
            }
        );

        glfwSetWindowCloseCallback(mHandle, [](GLFWwindow* window)
            {
                WindowDataFromCallback& data = *(WindowDataFromCallback*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.eventCallback(event);
            }
        );

        glfwSetKeyCallback(mHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowDataFromCallback& data = *(WindowDataFromCallback*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                    Input::UpdateKeyState(Input::mapKeys[key], true);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                    Input::UpdateKeyState(Input::mapKeys[key], false);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.eventCallback(event);
                    break;
                }
                }
            }
        );

        glfwSetMouseButtonCallback(mHandle, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowDataFromCallback& data = *(WindowDataFromCallback*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                    Input::UpdateMouseState(button, true);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.eventCallback(event);
                    Input::UpdateMouseState(button, false);
                    break;
                }
                }
            }
        );

        glfwSetScrollCallback(mHandle, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowDataFromCallback& data = *(WindowDataFromCallback*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.eventCallback(event);
            }
        );

        glfwSetCursorPosCallback(mHandle, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowDataFromCallback& data = *(WindowDataFromCallback*)glfwGetWindowUserPointer(window);

                MouseMovedEvent event((float)xPos, (float)yPos);
                data.eventCallback(event);
            });
    }
}
#endif