#include "aio_pch.h"

#include "Buffer.h"

#include "Alexio/Renderer.h"

#include "Renderer/OpenGL/OpenGL_Buffer.h"
#include "Renderer/DirectX11/DirectX11_Buffer.h"


namespace Alexio
{
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return std::make_shared<OpenGL_VertexBuffer>(size);
        case GraphicsAPI::DirectX11: return std::make_shared<DirectX11_VertexBuffer>(size);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return std::make_shared<OpenGL_VertexBuffer>(vertices, size);
        case GraphicsAPI::DirectX11: return std::make_shared<DirectX11_VertexBuffer>(vertices, size);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return std::make_shared<OpenGL_IndexBuffer>(indices, count);
        case GraphicsAPI::DirectX11: return std::make_shared<DirectX11_IndexBuffer>(indices, count);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    std::shared_ptr<VertexResources> VertexResources::Create()
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return std::make_shared<VertexArray>();
        case GraphicsAPI::DirectX11: return std::make_shared<DirectX11_VertexResources>();
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }
}


