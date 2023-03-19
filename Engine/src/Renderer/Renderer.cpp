#include "aio_pch.h"
#include "Renderer/Renderer.h"

namespace Alexio
{
	Ref<RendererBackend> Renderer::sRendererBackend = nullptr;
	Ref<ConstantBuffer> Renderer::sCameraBuffer = nullptr;
	Ref<QuadRenderer> Renderer::sQuadRenderer = nullptr;

	void Renderer::Init()
	{
		sRendererBackend = RendererBackend::Create();

		sRendererBackend->Initialize();

		sCameraBuffer = ConstantBuffer::Create(sizeof(glm::mat4x4), 0);

		sQuadRenderer = CreateRef<QuadRenderer>();

		sRendererBackend->SetVSync(true);
	}

	void Renderer::Draw(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const Ref<Texture>& texture)
	{
		vertexArray->Bind();
		shader->Bind();
		texture->Bind(0);

		sRendererBackend->DrawIndexed(vertexArray->GetIndexBuffer()->GetCount());

		texture->Unbind();
		shader->Unbind();
		vertexArray->Unbind();
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		sRendererBackend->Clear(r, g, b, a);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(position, 1.0f)) *
			glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));

		for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
		{
			sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->baseVertexPositions[i], 1.0f);
			sQuadRenderer->vertices[i].color = color;
		}

		sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());

		Draw(sQuadRenderer->shader, sQuadRenderer->vertexArray, sQuadRenderer->whiteTexture);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), position) *
			glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));

		for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
		{
			sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->baseVertexPositions[i], 1.0f);
			sQuadRenderer->vertices[i].color = color;
		}

		sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
		
		Draw(sQuadRenderer->shader, sQuadRenderer->vertexArray, sQuadRenderer->whiteTexture);
	}

	void Renderer::DrawSprite(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(position, 1.0f)) *
			glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));

		for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
		{
			sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->baseVertexPositions[i], 1.0f);
			sQuadRenderer->vertices[i].color = color;
		}

		sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
		
		Draw(sQuadRenderer->shader, sQuadRenderer->vertexArray, texture);
	}

	void Renderer::DrawSprite(const Ref<Texture>& texture, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), position) *
			glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));

		for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
		{
			sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->baseVertexPositions[i], 1.0f);
			sQuadRenderer->vertices[i].color = color;
		}

		sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
		
		Draw(sQuadRenderer->shader, sQuadRenderer->vertexArray, texture);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{	
		if (sRendererBackend != nullptr)
			sRendererBackend->SetViewport(x, y, width, height);
	}
}