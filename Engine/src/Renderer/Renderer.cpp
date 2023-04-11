#include "aio_pch.h"
#include "Renderer/Renderer.h"
#include "Alexio/Timer.h"

namespace Alexio
{
	Ref<RendererBackend> Renderer::sBackend = nullptr;
	Ref<ConstantBuffer>  Renderer::sCameraBuffer    = nullptr;
	Ref<LineRenderer>    Renderer::sLineRenderer    = nullptr;
	Renderer::Statistics Renderer::Stats = { 0 };

	static std::array<glm::vec3, 4> localPosition = 
	{
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3( 0.5f, -0.5f, 0.0f),
		glm::vec3( 0.5f,  0.5f, 0.0f),
		glm::vec3(-0.5f,  0.5f, 0.0f),
	}; 

	void Renderer::Init()
	{
		sBackend = RendererBackend::Create();
		sBackend->Initialize();

		sCameraBuffer = ConstantBuffer::Create(sizeof(Mat4x4), 0);

		LineRenderer::Init();
		QuadRenderer::Init();
		CircleRenderer::Init();

		StartBatches();

		sBackend->SetVSync(true);
	}

	void Renderer::StartBatches()
	{
		LineRenderer::StartNewBatch();
		QuadRenderer::StartNewBatch();
		CircleRenderer::StartNewBatch();
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		sBackend->Clear(r, g, b, a);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		if (sBackend != nullptr)
			sBackend->SetViewport(x, y, width, height);
	}

	void Renderer::Draw(uint32_t vertexCount)
	{
		sBackend->Draw(vertexCount);
	}

	void Renderer::DrawIndexed(uint32_t indexCount)
	{
		sBackend->DrawIndexed(indexCount);
	}

	void Renderer::Flush()
	{
		LineRenderer::SubmitBatch();
		QuadRenderer::SubmitBatch();
		CircleRenderer::SubmitBatch();
	}

	void Renderer::End()
	{
		LineRenderer::End();
		QuadRenderer::End();
		CircleRenderer::End();
	}

	void Renderer::DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color)
	{
		if (LineRenderer::LineCount >= LineRenderer::MaxLinesPerBatch)
			LineRenderer::SubmitBatch();
		
		LineRenderer::CurrentVertexPtr->position = glm::vec3(p0, 0.0f);
		LineRenderer::CurrentVertexPtr->color = color;
		LineRenderer::CurrentVertexPtr++;

		LineRenderer::CurrentVertexPtr->position = glm::vec3(p1, 0.0f);
		LineRenderer::CurrentVertexPtr->color = color;
		LineRenderer::CurrentVertexPtr++;

		LineRenderer::LineCount++;
		Stats.Lines++;
	}

	void Renderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{
		if (LineRenderer::LineCount >= LineRenderer::MaxLinesPerBatch)
			LineRenderer::SubmitBatch();

		LineRenderer::CurrentVertexPtr->position = p0;
		LineRenderer::CurrentVertexPtr->color = color;
		LineRenderer::CurrentVertexPtr++;

		LineRenderer::CurrentVertexPtr->position = p1;
		LineRenderer::CurrentVertexPtr->color = color;
		LineRenderer::CurrentVertexPtr++;

		LineRenderer::LineCount++;
		Stats.Lines++;
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
		glm::vec3 p0 = position;
		glm::vec3 p1 = { position.x + size.x, position.y, position.z };
		glm::vec3 p2 = { position.x + size.x, position.y + size.y, position.z };
		glm::vec3 p3 = { position.x, position.y + size.y, position.z };

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
		Stats.Quads++;
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
		Stats.Quads++;
	}
	
	void Renderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch)
			QuadRenderer::SubmitBatch();

		glm::vec2 originPoint = position + size * 0.5f;
		Mat4x4 transform = glm::translate(Mat4x4(1.0f), glm::vec3(originPoint, 1.0f)) *
			glm::rotate(Mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(Mat4x4(1.0f), glm::vec3(size, 1.0f));
		
		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[0], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;
		
		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[1], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;
		
		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[2], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;
		
		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[3], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;
		
		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;
		Stats.Quads++;
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		if (QuadRenderer::QuadCount >= QuadRenderer::MaxQuadsPerBatch)
			QuadRenderer::SubmitBatch();

		glm::vec3 originPoint = position + glm::vec3(size * 0.5f, 0.0f);
		Mat4x4 transform = glm::translate(Mat4x4(1.0f), originPoint) *
			glm::rotate(Mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(Mat4x4(1.0f), glm::vec3(size, 1.0f));

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[0], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[1], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[2], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[3], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = 0;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;
		Stats.Quads++;
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
		Stats.Quads++;

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
		Stats.Quads++;

		texture->Bind(texIndex);
		if (texIndex == QuadRenderer::TextureSlotIndex)
		{
			QuadRenderer::TextureIDs[QuadRenderer::TextureSlotIndex] = texture->GetID();
			QuadRenderer::TextureSlotIndex++;
		}
	}

	void Renderer::DrawPartialSprite(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& tileOffset, const glm::vec2& tileSize, const glm::vec4& colorTint)
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

		const std::array<glm::vec2, 4> textureCoordinates =
		{
			glm::vec2((tileOffset.x * tileSize.x)       / texture->GetWidth(), (tileOffset.y * tileSize.y)       / texture->GetHeight()),
			glm::vec2(((tileOffset.x + 1) * tileSize.x) / texture->GetWidth(), (tileOffset.y * tileSize.y)       / texture->GetHeight()),
			glm::vec2(((tileOffset.x + 1) * tileSize.x) / texture->GetWidth(), ((tileOffset.y + 1) * tileSize.y) / texture->GetHeight()),
			glm::vec2(((tileOffset.x) * tileSize.x)     / texture->GetWidth(), ((tileOffset.y + 1) * tileSize.y) / texture->GetHeight())
		};

		const glm::vec2 finalSize = glm::vec2(size.x * tileSize.x / texture->GetWidth(), size.y * tileSize.y / texture->GetHeight());

		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y, 0.0f };
		QuadRenderer::CurrentVertexPtr->color = colorTint;
		QuadRenderer::CurrentVertexPtr->texCoord = textureCoordinates[0];
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + finalSize.x, position.y, 0.0f };
		QuadRenderer::CurrentVertexPtr->color = colorTint;
		QuadRenderer::CurrentVertexPtr->texCoord = textureCoordinates[1];
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x + finalSize.x, position.y + finalSize.y, 0.0f };
		QuadRenderer::CurrentVertexPtr->color = colorTint;
		QuadRenderer::CurrentVertexPtr->texCoord = textureCoordinates[2];
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = { position.x, position.y + finalSize.y, 0.0f };
		QuadRenderer::CurrentVertexPtr->color = colorTint;
		QuadRenderer::CurrentVertexPtr->texCoord = textureCoordinates[3];
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;
		Stats.Quads++;

		texture->Bind(texIndex);
		if (texIndex == QuadRenderer::TextureSlotIndex)
		{
			QuadRenderer::TextureIDs[QuadRenderer::TextureSlotIndex] = texture->GetID();
			QuadRenderer::TextureSlotIndex++;
		}

	}

	void Renderer::DrawPartialSprite(const Ref<Texture>& texture, const glm::vec3& position, const glm::vec2& size, const glm::vec2& tileOffset, const glm::vec2& tileSize, const glm::vec4& colorTint)
	{
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
		Mat4x4 transform = glm::translate(Mat4x4(1.0f), glm::vec3(originPoint, 1.0f)) *
			glm::rotate(Mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(Mat4x4(1.0f), glm::vec3(size, 1.0f));

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[0], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[1], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[2], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[3], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;
		Stats.Quads++;

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
		Mat4x4 transform = glm::translate(Mat4x4(1.0f), originPoint) *
			glm::rotate(Mat4x4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::scale(Mat4x4(1.0f), glm::vec3(size, 1.0f));

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[0], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[1], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 0.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[2], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 1.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::CurrentVertexPtr->position = transform * glm::vec4(localPosition[3], 1.0f);
		QuadRenderer::CurrentVertexPtr->color = color;
		QuadRenderer::CurrentVertexPtr->texCoord = { 0.0f, 1.0f };
		QuadRenderer::CurrentVertexPtr->textureIndex = texIndex;
		QuadRenderer::CurrentVertexPtr++;

		QuadRenderer::IndexCount += 6;
		QuadRenderer::QuadCount++;
		Stats.Quads++;

		texture->Bind(texIndex);
		if (texIndex == QuadRenderer::TextureSlotIndex)
		{
			QuadRenderer::TextureIDs[QuadRenderer::TextureSlotIndex] = texture->GetID();
			QuadRenderer::TextureSlotIndex++;
		}
	}

	void Renderer::DrawCircle(const glm::vec2& position, const glm::vec4& color, float radius, float thickness, float fade)
	{
		if (CircleRenderer::CircleCount >= CircleRenderer::MaxCirclesPerBatch)
			CircleRenderer::SubmitBatch();

		CircleRenderer::CurrentVertexPtr->position = glm::vec4(position.x - radius, position.y - radius, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->localPosition = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->color = color;
		CircleRenderer::CurrentVertexPtr->thickness = thickness;
		CircleRenderer::CurrentVertexPtr->fade = fade;
		CircleRenderer::CurrentVertexPtr++;

		CircleRenderer::CurrentVertexPtr->position = glm::vec4(position.x + radius, position.y - radius, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->localPosition = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->color = color;
		CircleRenderer::CurrentVertexPtr->thickness = thickness;
		CircleRenderer::CurrentVertexPtr->fade = fade;
		CircleRenderer::CurrentVertexPtr++;

		CircleRenderer::CurrentVertexPtr->position = glm::vec4(position.x + radius, position.y + radius, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->localPosition = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->color = color;
		CircleRenderer::CurrentVertexPtr->thickness = thickness;
		CircleRenderer::CurrentVertexPtr->fade = fade;
		CircleRenderer::CurrentVertexPtr++;

		CircleRenderer::CurrentVertexPtr->position = glm::vec4(position.x - radius, position.y + radius, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->localPosition = glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->color = color;
		CircleRenderer::CurrentVertexPtr->thickness = thickness;
		CircleRenderer::CurrentVertexPtr->fade = fade;
		CircleRenderer::CurrentVertexPtr++;

		CircleRenderer::IndexCount += 6;
		CircleRenderer::CircleCount++;
		Stats.Circles++;
	}

	void Renderer::DrawCircle(const glm::vec3& position, const glm::vec4& color, float radius, float thickness, float fade)
	{
		if (CircleRenderer::CircleCount >= CircleRenderer::MaxCirclesPerBatch)
			CircleRenderer::SubmitBatch();

		CircleRenderer::CurrentVertexPtr->position = glm::vec4(position.x - radius, position.y - radius, position.z, 1.0f);
		CircleRenderer::CurrentVertexPtr->localPosition = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->color = color;
		CircleRenderer::CurrentVertexPtr->thickness = thickness;
		CircleRenderer::CurrentVertexPtr->fade = fade;
		CircleRenderer::CurrentVertexPtr++;

		CircleRenderer::CurrentVertexPtr->position = glm::vec4(position.x + radius, position.y - radius, position.z, 1.0f);
		CircleRenderer::CurrentVertexPtr->localPosition = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->color = color;
		CircleRenderer::CurrentVertexPtr->thickness = thickness;
		CircleRenderer::CurrentVertexPtr->fade = fade;
		CircleRenderer::CurrentVertexPtr++;

		CircleRenderer::CurrentVertexPtr->position = glm::vec4(position.x + radius, position.y + radius, position.z, 1.0f);
		CircleRenderer::CurrentVertexPtr->localPosition = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->color = color;
		CircleRenderer::CurrentVertexPtr->thickness = thickness;
		CircleRenderer::CurrentVertexPtr->fade = fade;
		CircleRenderer::CurrentVertexPtr++;

		CircleRenderer::CurrentVertexPtr->position = glm::vec4(position.x - radius, position.y + radius, position.z, 1.0f);
		CircleRenderer::CurrentVertexPtr->localPosition = glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
		CircleRenderer::CurrentVertexPtr->color = color;
		CircleRenderer::CurrentVertexPtr->thickness = thickness;
		CircleRenderer::CurrentVertexPtr->fade = fade;
		CircleRenderer::CurrentVertexPtr++;

		CircleRenderer::IndexCount += 6;
		CircleRenderer::CircleCount++;
		Stats.Circles++;
	}
}