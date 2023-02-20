#pragma once

#include "Renderer/OpenGL/OpenGL_Buffer.h"

namespace Alexio
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return mVertexBuffers; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return mIndexBuffer; }
	private:
		uint32_t mID;
		std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
	};
}