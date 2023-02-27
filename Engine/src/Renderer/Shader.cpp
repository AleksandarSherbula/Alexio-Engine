#include "aio_pch.h"
#include "Shader.h"

#include "Alexio/Renderer.h"

#include "Renderer/OpenGL/OpenGL_Shader.h"
#include "Renderer/DX11/DX11_Shader.h"

namespace Alexio
{
    Ref<Shader> Shader::Create(const std::string& name)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL:    return CreateRef<OpenGL_Shader>(name);
        case GraphicsAPI::DirectX11: return CreateRef<DX11_Shader>(name);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath)
	{
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL:    return CreateRef<OpenGL_Shader>(name, filepath);
        case GraphicsAPI::DirectX11: return CreateRef<DX11_Shader>(name, filepath);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
	}

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL:    return CreateRef<OpenGL_Shader>(name, vertexSrc, pixelSrc);
        case GraphicsAPI::DirectX11: return CreateRef<DX11_Shader>(name, vertexSrc, pixelSrc);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }
}


