#include "aio_pch.h"
#include "RenderData.h"
#include "Renderer.h"

namespace Alexio
{
    CircleVertex::CircleVertex()
    {
        this->position = glm::vec3(0.0f);
        this->localPosition = glm::vec3(0.0f);
        this->color = glm::vec4(0.0f);
        this->thickness = 0.0f;
        this->fade = 0.0f;
    }

    CircleVertex::CircleVertex(const glm::vec3& position, const glm::vec4& color, float thickness, float fade)
    {
        this->position = position;
        this->localPosition = position;
        this->color = color;
        this->thickness = thickness;
        this->fade = fade;
    }


    Ref<VertexArray>  QuadRenderer::VertexArray  = nullptr;
    Ref<VertexBuffer> QuadRenderer::VertexBuffer = nullptr;
    Ref<IndexBuffer>  QuadRenderer::IndexBuffer  = nullptr;
    Ref<Shader>       QuadRenderer::Shader       = nullptr;
    Ref<Texture>      QuadRenderer::WhiteTexture = nullptr;

    std::array<glm::vec4, 4> QuadRenderer::VertexPositions;

    size_t QuadRenderer::QuadCount  = 0;
    size_t QuadRenderer::IndexCount = 0;

    QuadVertex* QuadRenderer::CurrentVertexPtr = nullptr;
    QuadVertex* QuadRenderer::BaseVertexBuffer = nullptr;


    void QuadRenderer::Init()
    {
        VertexPositions[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        VertexPositions[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        VertexPositions[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        VertexPositions[3] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

        const size_t maxVertexCount = 4 * MaxQuadsPerBatch;
        const size_t maxIndexCount = 6 * MaxQuadsPerBatch;

        BaseVertexBuffer = new QuadVertex[maxVertexCount];

        uint32_t* indices = new uint32_t[maxIndexCount];

        int32_t indexOffset = 0;
        for (size_t i = 0; i < MaxQuadsPerBatch; i++)
        {
            indices[i * 6 + 0] = 0 + indexOffset;
            indices[i * 6 + 1] = 1 + indexOffset;
            indices[i * 6 + 2] = 2 + indexOffset;
            indices[i * 6 + 3] = 2 + indexOffset;
            indices[i * 6 + 4] = 3 + indexOffset;
            indices[i * 6 + 5] = 0 + indexOffset;
        
            indexOffset += 4;
        }

        VertexArray  = VertexArray::Create();
        VertexBuffer = VertexBuffer::Create(maxVertexCount * sizeof(QuadVertex));
        IndexBuffer  = IndexBuffer::Create(indices, maxIndexCount);
        delete[] indices;

        BufferLayout layout =
        {
            {ShaderDataType::Float3, "aPosition" },
            {ShaderDataType::Float4, "aColor"    },
            {ShaderDataType::Float2, "aTexCoord" }
        };
        VertexBuffer->SetLayout(layout);

        VertexArray->AddVertexBuffer(VertexBuffer);
        VertexArray->SetIndexBuffer(IndexBuffer);

        Shader = Shader::Create("quad", VertexArray);

        WhiteTexture = Texture::Create(1, 1, 0xffffffff);
	}

    void QuadRenderer::StartBatch()
    {
        QuadCount = 0;
        IndexCount = 0;

        CurrentVertexPtr = BaseVertexBuffer;
    }

    void QuadRenderer::SubmitBatch()
    {
        if (QuadCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)CurrentVertexPtr - (uint8_t*)BaseVertexBuffer);
            VertexBuffer->SetData(BaseVertexBuffer, dataSize);

            WhiteTexture->Bind(0);
            VertexArray->Bind();
            Shader->Bind();

            Renderer::DrawIndexed(IndexCount);

            Shader->Unbind();
            VertexArray->Unbind();
            WhiteTexture->Unbind();

            Renderer::DrawQuadCalls++;
        }
    }

    void QuadRenderer::SetNextBatch()
    {
        SubmitBatch();
        StartBatch();
    }

    CircleRenderer::CircleRenderer()
    {
        vertices[0] = CircleVertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f);
        vertices[1] = CircleVertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f);
        vertices[2] = CircleVertex(glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f);
        vertices[3] = CircleVertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f);

        indices =
        {
            0, 1, 2,
            2, 3, 0
        };

        vertexArray = VertexArray::Create();
        vertexBuffer = VertexBuffer::Create(vertices.data(), sizeof(CircleVertex) * 4);
        indexBuffer = IndexBuffer::Create(indices.data(), 6);

        BufferLayout layout =
        {
            {ShaderDataType::Float3, "aPosition"      },
            {ShaderDataType::Float3, "aLocalPosition" },
            {ShaderDataType::Float4, "aColor"         },
            {ShaderDataType::Float,  "aThickness"     },
            {ShaderDataType::Float,  "aFade"          }
        };
        vertexBuffer->SetLayout(layout);

        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);

        shader = Shader::Create("circle", vertexArray);
    }

    PointVertex::PointVertex()
    {
        this->position = glm::vec3(0.0f);
        this->color = glm::vec4(0.0f);
    }

    PointVertex::PointVertex(const glm::vec3& position, const glm::vec4& color)
    {
        this->position = position;
        this->color = color;
    }

    LineRenderer::LineRenderer()
    {
        vertices[0] = PointVertex(glm::vec3( 0.0f, 0.0f, 0.0f),  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        vertices[1] = PointVertex(glm::vec3( 0.5f, 0.0f, 0.0f),  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        vertexArray = VertexArray::Create();
        vertexBuffer = VertexBuffer::Create(vertices.data(), sizeof(PointVertex) * 2);

        BufferLayout layout =
        {
            {ShaderDataType::Float3, "aPosition" },
            {ShaderDataType::Float4, "aColor"    }
        };
        vertexBuffer->SetLayout(layout);

        vertexArray->AddVertexBuffer(vertexBuffer);

        shader = Shader::Create("line", vertexArray);
    }
}

