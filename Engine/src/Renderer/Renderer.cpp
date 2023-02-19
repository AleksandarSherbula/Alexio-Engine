#include "aio_pch.h"
#include "Alexio/Renderer.h"

#include <imgui.h>

namespace Alexio
{	
	std::shared_ptr<RendererAPI> Renderer::sRendererAPI = nullptr;
	ImGUI* Renderer::imgui = nullptr;
	GraphicsAPI Renderer::s_API = GraphicsAPI::OpenGL;

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
	}

	void Renderer::SetAPI(GraphicsAPI api)
	{
		s_API = api;

		switch (s_API)
		{
		case GraphicsAPI::OpenGL:     Window::SetAPI(WindowAPI::GLFW);  break;
		case GraphicsAPI::DirectX11:  Window::SetAPI(WindowAPI::Win32); break;
		}
	}
}