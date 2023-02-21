#pragma once

#include "Renderer/Buffer.h"

namespace Alexio
{
	class DirectX11_VertexBuffer : public VertexBuffer
	{
	public:
		DirectX11_VertexBuffer(uint32_t size);
		DirectX11_VertexBuffer(float* vertices, uint32_t size);

		~DirectX11_VertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size) override;
	private:
		uint32_t mID;
	};

	class DirectX11_IndexBuffer : public IndexBuffer
	{
	public:
		DirectX11_IndexBuffer(uint32_t* indices, uint32_t count);
		~DirectX11_IndexBuffer();

		void Bind() override;
		void Unbind() override;

		uint32_t GetCount() override;
	};

	class DirectX11_VertexData : public VertexData
	{
	public:
		DirectX11_VertexData();
		~DirectX11_VertexData();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	private:
		uint32_t mID;
	};

}