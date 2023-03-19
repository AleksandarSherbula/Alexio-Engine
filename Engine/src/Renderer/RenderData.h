#pragma once

#include "Math/Math.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"

#include <array>

namespace Alexio
{
	class QuadVertex
	{
	public:
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
	public:
		QuadVertex();
		QuadVertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& texCoord);
		
	private:
	};

	class QuadRenderer
	{
	public:
		std::array<QuadVertex, 4> vertices;
		std::array<uint32_t, 6> indices;
		std::array<glm::vec3, 4> baseVertexPositions;

		Ref<VertexArray>  vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer>  indexBuffer;
		Ref<Shader>       shader;
		Ref<Texture>      whiteTexture;
	public:
		QuadRenderer();
	};
}