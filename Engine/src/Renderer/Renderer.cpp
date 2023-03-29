#include "aio_pch.h"
#include "Renderer/Renderer.h"

namespace Alexio
{
	Ref<RendererBackend> Renderer::sRendererBackend = nullptr;
	Ref<ConstantBuffer>  Renderer::sCameraBuffer    = nullptr;
	Ref<LineRenderer>    Renderer::sLineRenderer    = nullptr;
	//Ref<CircleRenderer>  Renderer::sCircleRenderer  = nullptr;
	int32_t Renderer::DrawQuadCalls = 0;

	void Renderer::Init()
	{
		sRendererBackend = RendererBackend::Create();

		sRendererBackend->Initialize();

		sCameraBuffer = ConstantBuffer::Create(sizeof(glm::mat4x4), 0);

		QuadRenderer::Init();
		sLineRenderer = CreateRef<LineRenderer>();
		//sCircleRenderer = CreateRef<CircleRenderer>();

		StartBatches();

		sRendererBackend->SetVSync(true);
	}

	void Renderer::StartBatches()
	{
		QuadRenderer::StartBatch();
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

	void Renderer::Draw(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		shader->Bind();

		sRendererBackend->Draw(vertexCount);
	}

	void Renderer::DrawIndexed(uint32_t indexCount)
	{
		sRendererBackend->DrawIndexed(indexCount);
	}

	void Renderer::End()
	{
		delete[] QuadRenderer::BaseVertexBuffer;
	}

	void Renderer::DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color)
	{
		sLineRenderer->vertices[0].position = glm::vec3(p0, 0.0f);
		sLineRenderer->vertices[0].color = color;

		sLineRenderer->vertices[1].position = glm::vec3(p1, 0.0f);
		sLineRenderer->vertices[1].color = color;

		sLineRenderer->vertexBuffer->SetData(sLineRenderer->vertices.data(), sizeof(PointVertex) * sLineRenderer->vertices.size());

		Draw(sLineRenderer->shader, sLineRenderer->vertexArray, sLineRenderer->vertices.size());
	}

	void Renderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{
		sLineRenderer->vertices[0].position = p0;
		sLineRenderer->vertices[0].color = color;

		sLineRenderer->vertices[1].position = p1;
		sLineRenderer->vertices[1].color = color;

		sLineRenderer->vertexBuffer->SetData(sLineRenderer->vertices.data(), sizeof(PointVertex) * sLineRenderer->vertices.size());

		Draw(sLineRenderer->shader, sLineRenderer->vertexArray, sLineRenderer->vertices.size());
	}

	void Renderer::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::vec2 p0 = position;
		glm::vec2 p1 = { position.x + size.x, position.y};
		glm::vec2 p2 = { position.x + size.x, position.y + size.y};
		glm::vec2 p3 = { position.x, position.y + size.y};

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void Renderer::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::vec2 p0 = { position.x - size.x * 0.5f, position.y - size.y * 0.5f };
		glm::vec2 p1 = { position.x + size.x * 0.5f, position.y - size.y * 0.5f };
		glm::vec2 p2 = { position.x + size.x * 0.5f, position.y + size.y * 0.5f };
		glm::vec2 p3 = { position.x - size.x * 0.5f, position.y + size.y * 0.5f };

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch)
			QuadRenderer::SetNextBatch();

		glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(position, 1.0f)) *
			glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));

		const glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (int32_t i = 0; i < 4; i++)
		{
			QuadRenderer::CurrentVertexPtr->position = transform * QuadRenderer::VertexPositions[i];
			QuadRenderer::CurrentVertexPtr->color = color;
			QuadRenderer::CurrentVertexPtr->texCoord = textureCoords[i];
			QuadRenderer::CurrentVertexPtr++;
		}

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
	}

	//void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(position, 1.0f)) *
	//		glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));
	//
	//	for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
	//	{
	//		sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->localQuadPositions[i], 1.0f);
	//		sQuadRenderer->vertices[i].color = color;
	//	}
	//
	//	sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
	//
	//	sQuadRenderer->whiteTexture->Bind(0);
	//	DrawIndexed(QuadRenderer::Shader, QuadRenderer::VertexArray);
	//	sQuadRenderer->whiteTexture->Unbind();
	//
	//	QuadRenderer::IndexCount() += 6;
	//	QuadRenderer::QuadCount()++;
	//}
	//
	//void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), position) *
	//		glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));
	//
	//	for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
	//	{
	//		sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->localQuadPositions[i], 1.0f);
	//		sQuadRenderer->vertices[i].color = color;
	//	}
	//
	//	sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
	//
	//	sQuadRenderer->whiteTexture->Bind(0);
	//	DrawIndexed(QuadRenderer::Shader, QuadRenderer::VertexArray);
	//	sQuadRenderer->whiteTexture->Unbind();
	//
	//	QuadRenderer::QuadCount()++;
	//}
	//
	//void Renderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float angle)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(position, 1.0f)) *
	//		glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));
	//
	//	for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
	//	{
	//		sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->localQuadPositions[i], 1.0f);
	//		sQuadRenderer->vertices[i].color = color;
	//	}
	//
	//	sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
	//
	//	sQuadRenderer->whiteTexture->Bind(0);
	//	DrawIndexed(QuadRenderer::Shader, QuadRenderer::VertexArray);
	//	sQuadRenderer->whiteTexture->Unbind();
	//}
	//
	//void Renderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float angle)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), position) *
	//		glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));
	//
	//	for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
	//	{
	//		sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->localQuadPositions[i], 1.0f);
	//		sQuadRenderer->vertices[i].color = color;
	//	}
	//
	//	sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
	//	
	//	sQuadRenderer->whiteTexture->Bind(0);
	//	DrawIndexed(QuadRenderer::Shader, QuadRenderer::VertexArray);
	//	sQuadRenderer->whiteTexture->Unbind();
	//}
	//
	//void Renderer::DrawSprite(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(position, 1.0f)) *
	//		glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));
	//
	//	for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
	//	{
	//		sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->localQuadPositions[i], 1.0f);
	//		sQuadRenderer->vertices[i].color = color;
	//	}
	//
	//	sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
	//
	//	texture->Bind(0);
	//	DrawIndexed(QuadRenderer::Shader, QuadRenderer::VertexArray);
	//	texture->Unbind();
	//}
	//
	//void Renderer::DrawSprite(const Ref<Texture>& texture, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), position) *
	//		glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));
	//
	//	for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
	//	{
	//		sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->localQuadPositions[i], 1.0f);
	//		sQuadRenderer->vertices[i].color = color;
	//	}
	//
	//	sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
	//
	//	texture->Bind(0);
	//	DrawIndexed(QuadRenderer::Shader, QuadRenderer::VertexArray);
	//	texture->Unbind();
	//}
	//
	//void Renderer::DrawRotatedSprite(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float angle)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(position, 1.0f)) *
	//		glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));
	//
	//	for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
	//	{
	//		sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->localQuadPositions[i], 1.0f);
	//		sQuadRenderer->vertices[i].color = color;
	//	}
	//
	//	sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
	//	
	//	texture->Bind(0);
	//	DrawIndexed(QuadRenderer::Shader, QuadRenderer::VertexArray);
	//	texture->Unbind();
	//}
	//
	//void Renderer::DrawRotatedSprite(const Ref<Texture>& texture, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float angle)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), position) *
	//		glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));
	//
	//	for (int i = 0; i < sQuadRenderer->vertices.size(); i++)
	//	{
	//		sQuadRenderer->vertices[i].position = transform * glm::vec4(sQuadRenderer->localQuadPositions[i], 1.0f);
	//		sQuadRenderer->vertices[i].color = color;
	//	}
	//
	//	//sQuadRenderer->vertexBuffer->SetData(sQuadRenderer->vertices.data(), sizeof(QuadVertex) * sQuadRenderer->vertices.size());
	//	
	//	texture->Bind(0);
	//	DrawIndexed(QuadRenderer::Shader, QuadRenderer::VertexArray);
	//	texture->Unbind();
	//}

	//void Renderer::DrawCircle(const glm::vec2& position, const glm::vec4& color, float radius, float thickness, float fade)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(position, 1.0f)) *
	//		 glm::scale(glm::mat4x4(1.0f), glm::vec3(radius, radius, 1.0f));
	//
	//	for (int i = 0; i < sCircleRenderer->vertices.size(); i++)
	//	{
	//		sCircleRenderer->vertices[i].position  = transform * glm::vec4(sCircleRenderer->vertices[i].localPosition, 1.0f);
	//		sCircleRenderer->vertices[i].color     = color;
	//		sCircleRenderer->vertices[i].thickness = thickness;
	//		sCircleRenderer->vertices[i].fade      = fade;
	//	}
	//
	//	sCircleRenderer->vertexBuffer->SetData(sCircleRenderer->vertices.data(), sizeof(CircleVertex) * sCircleRenderer->vertices.size());
	//
	//	DrawIndexed(sCircleRenderer->shader, sCircleRenderer->vertexArray);
	//}
	//
	//void Renderer::DrawCircle(const glm::vec3& position, const glm::vec4& color, float radius, float thickness, float fade)
	//{
	//	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), position) *
	//		glm::scale(glm::mat4x4(1.0f), glm::vec3(radius, radius, 1.0f));
	//
	//	for (int i = 0; i < sCircleRenderer->vertices.size(); i++)
	//	{
	//		sCircleRenderer->vertices[i].position  = transform * glm::vec4(sCircleRenderer->vertices[i].localPosition, 1.0f);
	//		sCircleRenderer->vertices[i].color     = color;
	//		sCircleRenderer->vertices[i].thickness = thickness;
	//		sCircleRenderer->vertices[i].fade      = fade;
	//	}
	//
	//	sCircleRenderer->vertexBuffer->SetData(sCircleRenderer->vertices.data(), sizeof(CircleVertex) * sCircleRenderer->vertices.size());
	//
	//	DrawIndexed(sCircleRenderer->shader, sCircleRenderer->vertexArray);
	//}

	void Renderer::SubmitBatches()
	{
		QuadRenderer::SubmitBatch();
	}
}