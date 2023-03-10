#pragma once

#include "Renderer/Buffer.h"

namespace Alexio
{
	class DX11_VertexBuffer : public VertexBuffer
	{
	public:
		DX11_VertexBuffer(uint32_t size);
		DX11_VertexBuffer(float* vertices, uint32_t size);

		~DX11_VertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	};

	class DX11_IndexBuffer : public IndexBuffer
	{
	public:
		DX11_IndexBuffer(uint32_t* indices, uint32_t count);
		~DX11_IndexBuffer();

		void Bind() override;
		void Unbind() override;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	};

	class DX11_VertexResources : public VertexResources
	{
	public:
		DX11_VertexResources();
		~DX11_VertexResources();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

	private:
		uint32_t mID;
	};

	class DX11_ConstantBuffer : public ConstantBuffer
	{
	public:
		DX11_ConstantBuffer(uint32_t block_size, uint32_t binding);

		void SetData(const void* data, uint32_t data_size, uint32_t offset) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;		
	};
}