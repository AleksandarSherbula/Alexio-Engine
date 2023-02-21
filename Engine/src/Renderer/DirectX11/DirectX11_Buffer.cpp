#include "aio_pch.h"
#include "DirectX11_Buffer.h"

namespace Alexio
{
	DirectX11_VertexBuffer::DirectX11_VertexBuffer(uint32_t size)
	{
	}

	DirectX11_VertexBuffer::DirectX11_VertexBuffer(float* vertices, uint32_t size)
	{
	}

	DirectX11_VertexBuffer::~DirectX11_VertexBuffer()
	{
	}

	void DirectX11_VertexBuffer::Bind() const
	{
	}

	void DirectX11_VertexBuffer::Unbind() const
	{
	}

	void DirectX11_VertexBuffer::SetData(const void* data, uint32_t size)
	{
	}

	/////////////////////////////////////////////////////
	///////////INDEX BUFFER//////////////////////////////
	/////////////////////////////////////////////////////

	DirectX11_IndexBuffer::DirectX11_IndexBuffer(uint32_t* indices, uint32_t count)
	{
	}

	DirectX11_IndexBuffer::~DirectX11_IndexBuffer()
	{
	}

	void DirectX11_IndexBuffer::Bind()
	{
	}

	void DirectX11_IndexBuffer::Unbind()
	{
	}

	uint32_t DirectX11_IndexBuffer::GetCount()
	{
		return 0;
	}

	/////////////////////////////////////////////////////
	///////////VERTEX DATA///////////////////////////////
	/////////////////////////////////////////////////////

	DirectX11_VertexData::DirectX11_VertexData()
	{
	}

	DirectX11_VertexData::~DirectX11_VertexData()
	{
	}

	void DirectX11_VertexData::Bind() const
	{
	}

	void DirectX11_VertexData::Unbind() const
	{
	}

	void DirectX11_VertexData::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
	}

	void DirectX11_VertexData::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
	}
}


