#include "aio_pch.h"
#include "Texture.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGL_Texture.h"
#include "Renderer/DX11/DX11_Texture.h"

namespace Alexio
{
    Ref<Texture> Texture::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return CreateRef<OpenGL_Texture>(width, height);
        case GraphicsAPI::DirectX11: return CreateRef<DX11_Texture>(width, height);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    Ref<Texture> Texture::Create(const std::string& filepath)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return CreateRef<OpenGL_Texture>(filepath);
        case GraphicsAPI::DirectX11: return CreateRef<DX11_Texture>(filepath);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }
}