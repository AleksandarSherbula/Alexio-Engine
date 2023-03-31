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


    Ref<VertexArray>  QuadRenderer::vertexArray  = nullptr;
    Ref<VertexBuffer> QuadRenderer::vertexBuffer = nullptr;
    Ref<IndexBuffer>  QuadRenderer::indexBuffer  = nullptr;
    Ref<Shader>       QuadRenderer::shader       = nullptr;
    Ref<Texture>      QuadRenderer::WhiteTexture = nullptr;

    uint32_t QuadRenderer::QuadCount  = 0;
    uint32_t QuadRenderer::IndexCount = 0;
    uint32_t QuadRenderer::TextureSlotIndex = 0;

    std::array<uint32_t, QuadRenderer::MaxTextureSlots> QuadRenderer::TextureIDs = { 0 };

    QuadVertex* QuadRenderer::CurrentVertexPtr = nullptr;
    QuadVertex* QuadRenderer::baseVertexBuffer = nullptr;

    void QuadRenderer::Init()
    {
        const uint32_t maxVertexCount = 4 * MaxQuadsPerBatch;
        const uint32_t maxIndexCount = 6 * MaxQuadsPerBatch;

        baseVertexBuffer = new QuadVertex[maxVertexCount];

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

        vertexArray  = VertexArray::Create();
        vertexBuffer = VertexBuffer::Create(maxVertexCount * sizeof(QuadVertex));
        indexBuffer  = IndexBuffer::Create(indices, maxIndexCount);
        delete[] indices;

        BufferLayout layout =
        {
            {ShaderDataType::Float3, "aPosition" },
            {ShaderDataType::Float4, "aColor"    },
            {ShaderDataType::Float2, "aTexCoord" },
            {ShaderDataType::Int,    "aTexIndex" }
        };
        vertexBuffer->SetLayout(layout);

        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);

        shader = Shader::Create("quad", vertexArray);

        WhiteTexture = Texture::Create(1, 1, 0xffffffff);
        TextureIDs[TextureSlotIndex++] = WhiteTexture->GetID();
	}

    void QuadRenderer::StartNewBatch()
    {
        QuadCount = 0;
        IndexCount = 0;
        TextureSlotIndex = 1;

        CurrentVertexPtr = baseVertexBuffer;
    }

    void QuadRenderer::SubmitBatch()
    {
        if (QuadCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)CurrentVertexPtr - (uint8_t*)baseVertexBuffer);
            vertexBuffer->SetData(baseVertexBuffer, dataSize);

            vertexArray->Bind();
            shader->Bind();

            Renderer::DrawIndexed(IndexCount);

            shader->Unbind();
            vertexArray->Unbind();
        }



        StartNewBatch();
    }

    void QuadRenderer::End()
    {
        delete[] baseVertexBuffer;
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

