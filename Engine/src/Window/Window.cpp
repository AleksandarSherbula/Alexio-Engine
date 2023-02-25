#include "aio_pch.h"

#include "GLFW_Window.h"
#ifdef AIO_PLATFORM_WINDOWS
#include "Win32_Window.h"
#endif // AIO_PLATFORM_WINDOWS

namespace Alexio
{
    WindowAPI Window::sAPI = WindowAPI::GLFW;

    Window::~Window()
    {
    }

    std::shared_ptr<Window> Window::Create(const std::string& title, uint32_t width, uint32_t height)
    {
#ifdef AIO_PLATFORM_WINDOWS
        switch (sAPI)
        {
        case WindowAPI::GLFW:  return std::make_shared<GLFW_Window>(title, width, height);
        case WindowAPI::Win32: return std::make_shared<Win32_Window>(title, width, height);
        }
#else
        if (Renderer::GetGraphicsAPI() == GraphicsAPI::OpenGL)
            return std::make_shared<GLFW_Window>(title, width, height);
        else
            AIO_LOG_ERROR("DirectX11 is only supported on Windows");
#endif
        AIO_ASSERT(false, "Unknown Window API hase been selected");
        sAPI = WindowAPI::None;
        return nullptr;
    }
}