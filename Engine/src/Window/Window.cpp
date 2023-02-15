#include "aio_pch.h"

#include "GLFW_Window.h"
#ifdef AIO_PLATFORM_WINDOWS
#include "Win32_Window.h"
#endif // AIO_PLATFORM_WINDOWS

namespace Alexio
{
    WindowAPI Window::sAPI = WindowAPI::None;

    Window::~Window()
    {
    }

    std::unique_ptr<Window> Window::Create(const std::string& title, uint32_t width, uint32_t height, GraphicsAPIflag api)
    {
#ifdef AIO_PLATFORM_WINDOWS
        switch (api)
        {
        case GraphicsAPIflag::OpenGL:    sAPI = WindowAPI::GLFW; return std::make_unique<GLFW_Window>(title, width, height);
        case GraphicsAPIflag::DirectX11: sAPI = WindowAPI::Win32; return std::make_unique<Win32_Window>(title, width, height);
        }
#else
        if (api == GraphicsAPI::OpenGL)
            return std::make_unique<GLFW_Window>(title, width, height);
        else
            AIO_LOG_ERROR("DirectX11 is only supported on Windows");
#endif
        AIO_ASSERT(false, "A Graphics API has not been picked");
        return nullptr;
    }
}