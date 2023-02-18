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

		void Bind() override;
		void Unbind() override;

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

		inline uint32_t GetCount() override { return mCount; };
	private:
		uint32_t mID;
		uint32_t mCount;
	};

}