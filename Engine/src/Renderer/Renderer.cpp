#include "aio_pch.h"
#include "Alexio/Renderer.h"

#include <imgui.h>

namespace Alexio
{
	GraphicsAPIflag Renderer::sAPI_flag = GraphicsAPIflag::None;
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
		sRendererAPI->ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		imgui->Begin();

		imgui->OnUpdate();

		sRendererAPI->Draw();

		imgui->End();

		sRendererAPI->SwapBuffer();
	}

	void Renderer::End()
	{
		imgui->OnDetach();
		delete imgui;

		delete sRendererAPI;
	}
}