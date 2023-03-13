#include "aio_pch.h"
#include "Renderer/Renderer.h"
#include "Input/Input.h"

#include <imgui.h>

namespace Alexio
{	
	GraphicsAPI Renderer::s_API = GraphicsAPI::OpenGL;

	Ref<RendererAPI> Renderer::sRendererAPI = nullptr;
	
	Ref<ConstantBuffer> Renderer::sCameraBuffer = nullptr;	

	void Renderer::Begin(Window* window)
	{
		sRendererAPI = RendererAPI::Create();

		sRendererAPI->SetWindow(window);

		sRendererAPI->Initialize();		
		
		sCameraBuffer = ConstantBuffer::Create(sizeof(glm::mat4x4), 0);

		sRendererAPI->SetVSync(true);
	}

	void Renderer::Draw(const Ref<Shader>& shader, const Ref<VertexResources>& vertexResources)
	{
		vertexResources->Bind();
		shader->Bind();
		
		sRendererAPI->Draw(shader, vertexResources);
	}

	void Renderer::End()
	{
	}

	void Renderer::ClearColor(float r, float g, float b, float a)
	{
		sRendererAPI->ClearColor(r, g, b, a);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{	
		if (sRendererAPI != nullptr)
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