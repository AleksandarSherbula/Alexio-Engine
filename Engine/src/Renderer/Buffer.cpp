#include "aio_pch.h"

#include "Buffer.h"

#include "Alexio/Renderer.h"

#include "Renderer/OpenGL/OpenGL_Buffer.h"
#include "Renderer/DX11/DX11_Buffer.h"


namespace Alexio
{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return CreateRef<OpenGL_VertexBuffer>(size);
        case GraphicsAPI::DirectX11: return CreateRef<DX11_VertexBuffer>(size);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return CreateRef<OpenGL_VertexBuffer>(vertices, size);
        case GraphicsAPI::DirectX11: return CreateRef<DX11_VertexBuffer>(vertices, size);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return CreateRef<OpenGL_IndexBuffer>(indices, count);
        case GraphicsAPI::DirectX11: return CreateRef<DX11_IndexBuffer>(indices, count);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    Ref<VertexResources> VertexResources::Create()
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL: return CreateRef<VertexArray>();
        case GraphicsAPI::DirectX11: return CreateRef<DX11_VertexResources>();
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }

    Scope<ConstantBuffer> ConstantBuffer::Create(uint32_t block_size, uint32_t binding)
    {
        switch (Renderer::GetGraphicsAPI())
        {
        case GraphicsAPI::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
        case GraphicsAPI::OpenGL:    return CreateScope<UniformBuffer>(block_size, binding);
        case GraphicsAPI::DirectX11: return CreateScope<DX11_ConstantBuffer>(block_size, binding);
        }

        AIO_ASSERT(false, "Unknown Rendering API");
        return nullptr;
    }
}


