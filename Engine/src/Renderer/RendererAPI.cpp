#include "aio_pch.h"

#include "Renderer/Renderer.h"
#include "Alexio/Utilities.h"

#include "OpenGL/OpenGL_Renderer.h"
#ifdef AIO_PLATFORM_WINDOWS
	#include "DX11/DX11_Renderer.h"
#endif // AIO_PLATFORM_WINDOWS

namespace Alexio
{
    Ref<RendererAPI> RendererAPI::Create()
    {
		switch (Renderer::GetGraphicsAPI())
		{
		case GraphicsAPI::OpenGL:    return CreateRef<OpenGL_Renderer>();
		case GraphicsAPI::DirectX11: return CreateRef<DX11_Renderer>();
		}

		AIO_ASSERT(false, "Unknown API has been selected.\n");
		Renderer::SetGraphicsAPI(GraphicsAPI::None);
		return nullptr;
    }
}