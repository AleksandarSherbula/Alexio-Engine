#include "aio_pch.h"
#include "Renderer/Renderer.h"

namespace Alexio
{
	Ref<RendererBackend> Renderer::sRendererBackend = nullptr;
	Ref<ConstantBuffer> Renderer::sCameraBuffer = nullptr;

	void Renderer::Begin()
	{
		sRendererBackend = RendererBackend::Create();

		sRendererBackend->Initialize();

		sCameraBuffer = ConstantBuffer::Create(sizeof(glm::mat4x4), 0);

		sRendererBackend->SetVSync(true);
	}

	void Renderer::Draw(const Ref<Shader>& shader, const Ref<VertexResources>& vertexResources)
	{		
		sRendererBackend->Draw(shader, vertexResources);
	}

	void Renderer::End()
	{
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		sRendererBackend->Clear(r, g, b, a);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{	
		if (sRendererBackend != nullptr)
			sRendererBackend->SetViewport(x, y, width, height);
	}
}