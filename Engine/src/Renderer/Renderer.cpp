#include "aio_pch.h"
#include "Alexio/Renderer.h"

#include <imgui.h>

namespace Alexio
{	
	Ref<RendererAPI> Renderer::sRendererAPI = nullptr;
	GraphicsAPI Renderer::s_API = GraphicsAPI::OpenGL;
	Scope<Camera> Renderer::sCamera = nullptr;
	Scope<ConstantBuffer> Renderer::sProjectionBuffer = nullptr;

	void Renderer::Begin(Window* window)
	{
		sRendererAPI = RendererAPI::Create();

		sRendererAPI->SetWindow(window);

		sRendererAPI->Initialize();

		sCamera = CreateScope<Camera>(0.0f, (float)window->GetWidth(), (float)window->GetHeight(), 0.0f);
		
		sProjectionBuffer = ConstantBuffer::Create(sizeof(glm::mat4x4), 0);

		sRendererAPI->SetVSync(true);
	}

	void Renderer::Draw(const Ref<Shader>& shader, const Ref<VertexResources>& vertexResources)
	{
		vertexResources->Bind();
		shader->Bind();
		
		//test code ----- yet again
		glm::vec2 position = { 0.0f, 0.0f };
		glm::vec2 size = { 200.0f, 200.f };
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));
		
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		
		model = glm::scale(model, glm::vec3(size, 1.0f));

		glm::mat4x4 modProjection = sCamera->GetProjection() * model;
		sProjectionBuffer->SetData(&modProjection, sizeof(glm::mat4x4));
		sProjectionBuffer->Bind(0);
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