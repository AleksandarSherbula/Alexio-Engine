#include "aio_pch.h"

#include "Renderer/Renderer.h"

#include "Buffer.h"
#include "Renderer/OpenGL/OpenGL_Buffer.h"
#include "Renderer/DX11/DX11_Buffer.h"

namespace Alexio
{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
            switch (Renderer::GetGraphicsAPI())
            {
                case OpenGL:    return CreateRef<OpenGL_VertexBuffer>(size);
                case DirectX11: return CreateRef<DX11_VertexBuffer>(size);
            }
        #elif defined(AIO_API_OPENGL)
            return CreateRef<OpenGL_VertexBuffer>(size);
        #elif defined(AIO_API_DX11)
            return CreateRef<DX11_VertexBuffer>(size);
        #endif
            AIO_ASSERT(false, "API has not been selected.\n");
            return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
            switch (Renderer::GetGraphicsAPI())
            {
                case OpenGL:    return CreateRef<OpenGL_VertexBuffer>(vertices, size);
                case DirectX11: return CreateRef<DX11_VertexBuffer>(vertices, size);
            }
        #elif defined(AIO_API_OPENGL)
            return CreateRef<OpenGL_VertexBuffer>(vertices, size);
        #elif defined(AIO_API_DX11)
            return CreateRef<DX11_VertexBuffer>(vertices, size);
        #endif
            AIO_ASSERT(false, "API has not been selected.\n");
            return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
            switch (Renderer::GetGraphicsAPI())
            {
                case OpenGL:    return CreateRef<OpenGL_IndexBuffer>(indices, count);
                case DirectX11: return CreateRef<DX11_IndexBuffer>(indices, count);
            }
        #elif defined(AIO_API_OPENGL)
            return CreateRef<OpenGL_IndexBuffer>(indices, count);
        #elif defined(AIO_API_DX11)
            return CreateRef<DX11_IndexBuffer>(indices, count);
        #endif
            AIO_ASSERT(false, "API has not been selected.\n");
            return nullptr;
    }

    Ref<VertexResources> VertexResources::Create()
    {
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
            switch (Renderer::GetGraphicsAPI())
            {
                case OpenGL:    return CreateRef<VertexArray>();
                case DirectX11: return CreateRef<DX11_VertexResources>();
            }
        #elif defined(AIO_API_OPENGL)
            return CreateRef<VertexArray>();
        #elif defined(AIO_API_DX11)
            return CreateRef<DX11_VertexResources>();
        #endif
            AIO_ASSERT(false, "API has not been selected.\n");
            return nullptr;
    }

    Ref<ConstantBuffer> ConstantBuffer::Create(uint32_t block_size, uint32_t slot)
    {
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
            switch (Renderer::GetGraphicsAPI())
            {
                case OpenGL:    return CreateRef<UniformBuffer>(block_size, slot);
                case DirectX11: return CreateRef<DX11_ConstantBuffer>(block_size, slot);
            }
        #elif defined(AIO_API_OPENGL)
            return CreateRef<UniformBuffer>(block_size, slot);
        #elif defined(AIO_API_DX11)
            return CreateRef<DX11_ConstantBuffer>(block_size, slot);
        #endif
            AIO_ASSERT(false, "API has not been selected.\n");
            return nullptr;
    }
}


