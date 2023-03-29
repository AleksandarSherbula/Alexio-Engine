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

		static Ref<VertexArray>  VertexArray;
		static Ref<VertexBuffer> VertexBuffer;
		static Ref<IndexBuffer>  IndexBuffer;
		static Ref<Shader>       Shader;
		static Ref<Texture>      WhiteTexture;

		static std::array<glm::vec4, 4> VertexPositions;

		static size_t QuadCount;
		static size_t IndexCount;

		static QuadVertex* CurrentVertexPtr;
		static QuadVertex* BaseVertexBuffer;

		static const size_t MaxQuadsPerBatch = 1000;
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