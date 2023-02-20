#include "aio_pch.h"
#include "Shader.h"

#include "Alexio/Renderer.h"

#include "Renderer/OpenGL/OpenGL_Shader.h"

namespace Alexio
{
	std::unique_ptr<Shader> Shader::Create(const std::string& vertexSrc, const std::string& pixelSrc)
	{
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return std::make_unique<OpenGL_Shader>(vertexSrc, pixelSrc);
        //case GraphicsAPIflag::DirectX11: return new Renderer_DirectX11();
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
	}
}


