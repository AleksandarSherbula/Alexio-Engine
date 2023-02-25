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
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	};

	class DirectX11_IndexBuffer : public IndexBuffer
	{
	public:
		DirectX11_IndexBuffer(uint32_t* indices, uint32_t count);
		~DirectX11_IndexBuffer();

		void Bind() override;
		void Unbind() override;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	};

	class DirectX11_VertexResources : public VertexResources
	{
	public:
		DirectX11_VertexResources();
		~DirectX11_VertexResources();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	private:
		uint32_t mID;
	};

}