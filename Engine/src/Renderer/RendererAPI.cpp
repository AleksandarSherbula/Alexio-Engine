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
		switch (Renderer::GetAPIflag())
		{
		case GraphicsAPIflag::None: AIO_ASSERT(false, "The Graphics API has not been seleceted"); break;
		case GraphicsAPIflag::OpenGL: return  std::make_shared<Renderer_OpenGL>();
		case GraphicsAPIflag::DirectX11: return std::make_shared<Renderer_DirectX11>();
		}

		AIO_ASSERT(false, "Error in selecting a Graphics API for Rendering.\n");
		return nullptr;
    }
}