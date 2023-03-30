#pragma once

#include "Math/Math.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"

#include <array>

namespace Alexio
{
	class PointVertex
	{
	public:
		glm::vec3 position;
		glm::vec4 color;
	public:
		PointVertex();
		PointVertex(const glm::vec3& position, const glm::vec4& color);
	};

	struct QuadVertex
	{	
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float textureIndex;
	};
		
	class CircleVertex
	{
	public:
		glm::vec3 position;
		glm::vec3 localPosition;
		glm::vec4 color;
		float thickness;
		float fade;
	public:
		CircleVertex();
		CircleVertex(const glm::vec3& position, const glm::vec4& color, float thickness, float fade);
	};

	class LineRenderer
	{
	public:
		std::array<PointVertex, 2> vertices;

		Ref<VertexArray>  vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader>       shader;
	public:
		LineRenderer();
	};

	class QuadRenderer
	{
	public:
		static void Init();
		static void StartBatch();
		static void SubmitBatch();
		static void SetNextBatch();
		static void End();
	public:
		static uint32_t QuadCount;
		static uint32_t IndexCount;
		static uint32_t TextureSlotIndex;

		static QuadVertex* CurrentVertexPtr;
		static const size_t MaxQuadsPerBatch = 1000;
		static const uint32_t MaxTextureSlots = 32;
		static std::array<uint32_t, MaxTextureSlots> TextureIDs;

		static Ref<Texture>      WhiteTexture;
	private:
		static Ref<VertexArray>  vertexArray;
		static Ref<VertexBuffer> vertexBuffer;
		static Ref<IndexBuffer>  indexBuffer;
		static Ref<Shader>       shader;

		static QuadVertex* baseVertexBuffer;
	};

	class CircleRenderer
	{
	public:
		std::array<CircleVertex, 4> vertices;
		std::array<uint32_t, 6> indices;

		Ref<VertexArray>  vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer>  indexBuffer;
		Ref<Shader>       shader;
	public:
		CircleRenderer();
	};
}