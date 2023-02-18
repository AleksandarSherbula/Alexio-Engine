#pragma once

#include <memory>

namespace Alexio
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		static std::unique_ptr<VertexBuffer> Create(uint32_t size);
		static std::unique_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetCount() = 0;

		static std::unique_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};

}