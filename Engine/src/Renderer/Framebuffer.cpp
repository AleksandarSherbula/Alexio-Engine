#include "aio_pch.h"

#include "Framebuffer.h"
#include "Renderer/OpenGL/OpenGL_Framebuffer.h"
#include "Renderer/DX11/DX11_Framebuffer.h"

namespace Alexio
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
#if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
        switch (Renderer::GetGraphicsAPI())
        {
        case OpenGL:    return CreateRef<OpenGL_Framebuffer>(spec);
        case DirectX11: return CreateRef<DX11_Framebuffer>(spec);
        }
#elif defined(AIO_API_OPENGL)
        return CreateRef<OpenGL_Framebuffer>(spec);
#elif defined(AIO_API_DX11)
        return CreateRef<DX11_Framebuffer>(spec);
#endif
        AIO_ASSERT(false, "API has not been selected.\n");
        return nullptr;
	}
}

