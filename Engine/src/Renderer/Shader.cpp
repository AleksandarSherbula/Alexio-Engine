#include "aio_pch.h"
#include "Shader.h"

#include "Alexio/Renderer.h"

#include "Renderer/OpenGL/OpenGL_Shader.h"
#include "Renderer/DirectX11/DirectX11_Shader.h"

namespace Alexio
{
    std::shared_ptr<Shader> Shader::Create(const std::string& name)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return    std::make_shared<OpenGL_Shader>(name);
        case GraphicsAPI::DirectX11: return std::make_shared<DirectX11_Shader>(name);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& filepath)
	{
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return    std::make_shared<OpenGL_Shader>(name, filepath);
        case GraphicsAPI::DirectX11: return std::make_shared<DirectX11_Shader>(name, filepath);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
	}

    std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return    std::make_shared<OpenGL_Shader>(name, vertexSrc, pixelSrc);
        case GraphicsAPI::DirectX11: return std::make_shared<DirectX11_Shader>(name, vertexSrc, pixelSrc);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }
}


