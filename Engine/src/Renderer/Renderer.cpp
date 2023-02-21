#include "aio_pch.h"
#include "Alexio/Renderer.h"

#include <imgui.h>

namespace Alexio
{	
	std::unique_ptr<RendererAPI> Renderer::sRendererAPI = nullptr;
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

	void Renderer::Draw(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexData>& vertexData)
	{
		sRendererAPI->Draw(shader, vertexData);
	}

	void Renderer::End()
	{
		imgui->OnDetach();
		delete imgui;
	}

	void Renderer::ClearColor(float r, float g, float b, float a)
	{
		sRendererAPI->ClearColor(r, g, b, a);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		sRendererAPI->SetViewport(x, y, width, height);
	}

	void Renderer::SwapBuffer()
	{
		sRendererAPI->SwapBuffer();
	}

	void Renderer::SetGraphicsAPI(GraphicsAPI api)
	{
		s_API = api;

		switch (s_API)
		{
		case GraphicsAPI::OpenGL:     Window::SetAPI(WindowAPI::GLFW);  break;
		case GraphicsAPI::DirectX11:  Window::SetAPI(WindowAPI::Win32); break;
		}
	}
}