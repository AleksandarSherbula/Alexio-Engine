#include "aio_pch.h"

#include "Buffer.h"

#include "Alexio/Renderer.h"

#include "Renderer/OpenGL/OpenGL_Buffer.h"


namespace Alexio
{
    std::unique_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return std::make_unique<OpenGL_VertexBuffer>(size);
        //case GraphicsAPIflag::DirectX11: return new Renderer_DirectX11();
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    std::unique_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return std::make_unique<OpenGL_VertexBuffer>(vertices, size);
        //case GraphicsAPIflag::DirectX11: return new Renderer_DirectX11();
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    std::unique_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return std::make_unique<OpenGL_IndexBuffer>(indices, count);
        //case GraphicsAPIflag::DirectX11: return new Renderer_DirectX11();
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }
}


