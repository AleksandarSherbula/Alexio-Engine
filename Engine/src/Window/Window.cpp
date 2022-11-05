#include "aio_pch.h"
#include "GLFW_Window.h"

#ifdef AIO_PLATFORM_WINDOWS
    #include "Win32_Window.h"
#endif // AIO_PLATFORM_WINDOWS


namespace Alexio
{
    std::unique_ptr<Window> Window::Create(const std::string& title, uint32_t width, uint32_t height, GraphicsAPI api)
    {
        if (api == GraphicsAPI::OpenGL)
            return std::make_unique<GLFW_Window>(title, width, height);
        else if (api == GraphicsAPI::DirectX11)
        {
            #ifndef AIO_PLATFORM_WINDOWS
                #error Win32 API is only supported on Windos
            #endif // AIO_PLATFORM_WINDOWS
            return std::make_unique<Win32_Window>(title, width, height);
        }

        AIO_ASSERT(false, "GLFW window is the only one that can be created.");
        return nullptr;
    }
}