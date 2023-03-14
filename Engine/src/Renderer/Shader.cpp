#include "aio_pch.h"
#include "Shader.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGL_Shader.h"
#include "Renderer/DX11/DX11_Shader.h"

namespace Alexio
{
    Ref<Shader> Shader::Create(const std::string& name)
    {
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
            switch (Renderer::GetGraphicsAPI())
            {
            case OpenGL:    return CreateRef<OpenGL_Shader>(name);
            case DirectX11: return CreateRef<DX11_Shader>(name);
            }
        #elif defined(AIO_API_OPENGL)
            return CreateRef<OpenGL_Shader>(name);
        #elif defined(AIO_API_DX11)
            return CreateRef<DX11_Shader>(name);
        #endif
            AIO_ASSERT(false, "API has not been selected.\n");
            return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath)
	{
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
            switch (Renderer::GetGraphicsAPI())
            {
            case OpenGL:    return CreateRef<OpenGL_Shader>(name, filepath);
            case DirectX11: return CreateRef<DX11_Shader>(name, filepath);
            }
        #elif defined(AIO_API_OPENGL)
            return CreateRef<OpenGL_Shader>(name, filepath);
        #elif defined(AIO_API_DX11)
            return CreateRef<DX11_Shader>(name, filepath);
        #endif
            AIO_ASSERT(false, "API has not been selected.\n");
            return nullptr;
	}

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc)
    {
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
            switch (Renderer::GetGraphicsAPI())
            {
            case OpenGL:    return CreateRef<OpenGL_Shader>(name, vertexSrc, pixelSrc);
            case DirectX11: return CreateRef<DX11_Shader>(name, vertexSrc, pixelSrc);
            }
        #elif defined(AIO_API_OPENGL)
            return CreateRef<OpenGL_Shader>(name, vertexSrc, pixelSrc);
        #elif defined(AIO_API_DX11)
            return CreateRef<DX11_Shader>(name, vertexSrc, pixelSrc);
        #endif
            AIO_ASSERT(false, "API has not been selected.\n");
            return nullptr;
    }
}


