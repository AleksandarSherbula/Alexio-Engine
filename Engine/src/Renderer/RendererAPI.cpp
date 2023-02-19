#include "aio_pch.h"

#include "Alexio/Renderer.h"
#include "Alexio/Utilities.h"

#include "OpenGL/OpenGL_Renderer.h"
#ifdef AIO_PLATFORM_WINDOWS
	#include "DirectX11/DirectX11_Renderer.h"
#endif // AIO_PLATFORM_WINDOWS

namespace Alexio
{
    std::shared_ptr<RendererAPI> RendererAPI::Create()
    {
		switch (Renderer::GetGraphicsAPI())
		{
		case GraphicsAPI::OpenGL:    return  std::make_shared<Renderer_OpenGL>();
		case GraphicsAPI::DirectX11: return std::make_shared<Renderer_DirectX11>();
		}

		AIO_ASSERT(false, "Unknown API has been selected.\n");
		Renderer::SetAPI(GraphicsAPI::None);
		return nullptr;
    }
}