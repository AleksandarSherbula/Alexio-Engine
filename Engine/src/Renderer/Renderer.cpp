#include "aio_pch.h"
#include "Alexio/Renderer.h"

namespace Alexio
{
	GraphicsAPI Renderer::sAPI_flag = GraphicsAPI::None;
	RendererAPI* Renderer::sRendererAPI = nullptr;

	void Renderer::Begin(Window* window)
	{
		sAPI_flag = GetAPI();
		sRendererAPI = RendererAPI::Create();

		sRendererAPI->SetWindow(window);
		sRendererAPI->Initialize();
		sRendererAPI->SetVSync(true);
	}

	void Renderer::DrawFrame()
	{
		sRendererAPI->ClearColor(0.0f, 0.8f, 1.0f, 1.0f);
		sRendererAPI->SwapBuffer();
	}

	void Renderer::End()
	{
		delete sRendererAPI;
	}
}