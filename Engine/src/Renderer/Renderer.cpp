#include "aio_pch.h"
#include "Alexio/Renderer.h"

namespace Alexio
{
	GraphicsAPI Renderer::sAPI_flag = GraphicsAPI::None;
	RendererAPI* Renderer::sRendererAPI = nullptr;
	ImGUI* Renderer::imgui = nullptr;

	void Renderer::Begin(Window* window)
	{
		sRendererAPI = RendererAPI::Create();

		sRendererAPI->SetWindow(window);
		sRendererAPI->Initialize();
		sRendererAPI->SetVSync(true);

		imgui = new ImGUI();
		imgui->OnAttach();

	}

	void Renderer::DrawFrame()
	{
		sRendererAPI->ClearColor(0.0f, 0.8f, 1.0f, 1.0f);

		imgui->Begin();

		imgui->OnUpdate();

		sRendererAPI->SwapBuffer();
	}

	void Renderer::End()
	{
		imgui->OnDetach();

		delete imgui;
		delete sRendererAPI;
	}
}