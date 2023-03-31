#include "aio_pch.h"
#include "Renderer/Renderer.h"
#include "Alexio/Timer.h"

namespace Alexio
{
	Ref<RendererBackend> Renderer::sRendererBackend = nullptr;
	Ref<ConstantBuffer>  Renderer::sCameraBuffer    = nullptr;
	Ref<LineRenderer>    Renderer::sLineRenderer    = nullptr;
	//Ref<CircleRenderer>  Renderer::sCircleRenderer  = nullptr;
	int32_t Renderer::DrawQuadCallCount = 1;

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
		QuadRenderer::StartNewBatch();
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
		QuadRenderer::End();
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
			QuadRenderer::SubmitBatch();
		
		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y, 0.0f };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + size.x, position.y, 0.0f };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y + size.y, 0.0f };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;
		
		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;

		QuadRenderer::WhiteTexture->Bind(0);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch)
			QuadRenderer::SubmitBatch();

		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y, position.z };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + size.x, position.y, position.z };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + size.x, position.y + size.y, position.z };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y + size.y, position.z };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;

		QuadRenderer::WhiteTexture->Bind(0);
	}
	
	void Renderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch)
			QuadRenderer::SubmitBatch();

		glm::vec2 originPoint = position + size * 0.5f;
		glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(originPoint, 1.0f)) *
			glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));
		
		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(-0.5f, -0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;
		
		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(0.5f, -0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;
		
		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;
		
		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(-0.5f, 0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;
		
		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;

		QuadRenderer::WhiteTexture->Bind(0);
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch)
			QuadRenderer::SubmitBatch();

		glm::vec3 originPoint = position + glm::vec3(size * 0.5f, 0.0f);
		glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), originPoint) *
			glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(-0.5f, -0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(0.5f, -0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(-0.5f, 0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;

		QuadRenderer::WhiteTexture->Bind(0);
	}

	void Renderer::DrawSprite(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch ||
			QuadRenderer::TextureSlotIndex >= QuadRenderer::MaxTextureSlots)
			QuadRenderer::SubmitBatch();

		uint32_t texIndex = QuadRenderer::TextureSlotIndex;
		for (int i = 1; i < QuadRenderer::TextureSlotIndex; i++)
		{
			if (QuadRenderer::TextureIDs[i] == texture->GetID())
			{
				texIndex = i;
				break;
			}
		}

		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y, 1.0f };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + size.x, position.y, 1.0f };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + size.x, position.y + size.y, 1.0f };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y + size.y, 1.0f };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;

		texture->Bind(texIndex);
		if (texIndex == QuadRenderer::TextureSlotIndex)
		{
			QuadRenderer::TextureIDs[QuadRenderer::TextureSlotIndex] = texture->GetID();
			QuadRenderer::TextureSlotIndex++;
		}
	}
	
	void Renderer::DrawSprite(const Ref<Texture>& texture, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch ||
			QuadRenderer::TextureSlotIndex >= QuadRenderer::MaxTextureSlots)
			QuadRenderer::SubmitBatch();

		uint32_t texIndex = QuadRenderer::TextureSlotIndex;
		for (int i = 1; i < QuadRenderer::TextureSlotIndex; i++)
		{
			if (QuadRenderer::TextureIDs[i] == texture->GetID())
			{
				texIndex = i;
				break;
			}
		}

		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y, position.z };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + size.x, position.y, position.z };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + size.x, position.y + size.y, position.z };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y + size.y, position.z };
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;

		texture->Bind(texIndex);
		if (texIndex == QuadRenderer::TextureSlotIndex)
		{
			QuadRenderer::TextureIDs[QuadRenderer::TextureSlotIndex] = texture->GetID();
			QuadRenderer::TextureSlotIndex++;
		}
	}
	
	void Renderer::DrawRotatedSprite(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch ||
			QuadRenderer::TextureSlotIndex >= QuadRenderer::MaxTextureSlots)
			QuadRenderer::SubmitBatch();

		uint32_t texIndex = QuadRenderer::TextureSlotIndex;
		for (int i = 1; i < QuadRenderer::TextureSlotIndex; i++)
		{
			if (QuadRenderer::TextureIDs[i] == texture->GetID())
			{
				texIndex = i;
				break;
			}
		}
		
		glm::vec2 originPoint = position + size * 0.5f;
		glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(originPoint, 1.0f)) *
			glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(-0.5f, -0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4( 0.5f, -0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4( 0.5f,  0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(-0.5f,  0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;

		texture->Bind(texIndex);
		if (texIndex == QuadRenderer::TextureSlotIndex)
		{
			QuadRenderer::TextureIDs[QuadRenderer::TextureSlotIndex] = texture->GetID();
			QuadRenderer::TextureSlotIndex++;
		}
	}
	
	void Renderer::DrawRotatedSprite(const Ref<Texture>& texture, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch ||
			QuadRenderer::TextureSlotIndex >= QuadRenderer::MaxTextureSlots)
			QuadRenderer::SubmitBatch();

		uint32_t texIndex = QuadRenderer::TextureSlotIndex;
		for (int i = 1; i < QuadRenderer::TextureSlotIndex; i++)
		{
			if (QuadRenderer::TextureIDs[i] == texture->GetID())
			{
				texIndex = i;
				break;
			}
		}

		glm::vec3 originPoint = position + glm::vec3(size * 0.5f, 0.0f);
		glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), originPoint) *
			glm::rotate(glm::mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4x4(1.0f), glm::vec3(size, 1.0f));

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(-0.5f, -0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(0.5f, -0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(-0.5f, 0.5f, 1.0f, 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;

		texture->Bind(texIndex);
		if (texIndex == QuadRenderer::TextureSlotIndex)
		{
			QuadRenderer::TextureIDs[QuadRenderer::TextureSlotIndex] = texture->GetID();
			QuadRenderer::TextureSlotIndex++;
		}
	}

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