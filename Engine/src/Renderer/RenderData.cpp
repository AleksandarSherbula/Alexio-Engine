#include "aio_pch.h"
#include "RenderData.h"


namespace Alexio
{
	QuadVertex::QuadVertex()
	{
		this->position = glm::vec3(0.0f);
		this->color = glm::vec4(0.0f);
		this->texCoord = glm::vec2(0.0f);
	}

	QuadVertex::QuadVertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& texCoord)
	{
		this->position = position;
		this->color = color;
		this->texCoord = texCoord;
	}

	QuadRenderer::QuadRenderer()
	{
        vertices[0] = QuadVertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f));
        vertices[1] = QuadVertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f));
        vertices[2] = QuadVertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f));
        vertices[3] = QuadVertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f));
        
        baseVertexPositions[0] = vertices[0].position;
        baseVertexPositions[1] = vertices[1].position;
        baseVertexPositions[2] = vertices[2].position;
        baseVertexPositions[3] = vertices[3].position;
        
        indices =
        {
            0, 1, 2,
            2, 3, 0
        };
        
        vertexArray = VertexArray::Create();
        vertexBuffer = VertexBuffer::Create(vertices.data(), sizeof(QuadVertex) * 4);
        indexBuffer = IndexBuffer::Create(indices.data(), 6);
        
        BufferLayout layout =
        {
            {ShaderDataType::Float3, "aPosition" },
            {ShaderDataType::Float4, "aColor"    },
            {ShaderDataType::Float2, "aTexCoord" }
        };
        vertexBuffer->SetLayout(layout);
        
        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);
        
        shader = Shader::Create("quad", vertexArray);

        whiteTexture = Texture::Create(1, 1, 0xffffffff);
	}
}
