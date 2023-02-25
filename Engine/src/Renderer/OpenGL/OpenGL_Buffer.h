#pragma once

#include "Renderer/Buffer.h"

namespace Alexio
{
	class OpenGL_VertexBuffer : public VertexBuffer
	{
	public:
		OpenGL_VertexBuffer(uint32_t size);
		OpenGL_VertexBuffer(float* vertices, uint32_t size);

		~OpenGL_VertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size) override;
	private:
		uint32_t mID;
		
	};

	class OpenGL_IndexBuffer : public IndexBuffer
	{
	public:
		OpenGL_IndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGL_IndexBuffer();

		void Bind() override;
		void Unbind() override;

	private:
		uint32_t mID;
	};

	class VertexArray : public VertexResources
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	private:
		uint32_t mID;
	};

}