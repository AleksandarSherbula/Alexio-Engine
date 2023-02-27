#include "aio_pch.h"
#include "DX11_Buffer.h"
#include "Alexio/Renderer.h"
#include "DX11_Renderer.h"

namespace Alexio
{
	DX11_VertexBuffer::DX11_VertexBuffer(uint32_t size)
	{
		// Possible subject to change code
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = size;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

		HRESULT hr = dynamic_cast<DX11_Renderer*>(Renderer::GetAPI())->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, mBuffer.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer: " + ResultInfo(hr) + "\n");
	}

	DX11_VertexBuffer::DX11_VertexBuffer(float* vertices, uint32_t size)
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = size;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = vertices;

		HRESULT hr = dynamic_cast<DX11_Renderer*>(Renderer::GetAPI())->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, mBuffer.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer: " + ResultInfo(hr) + "\n");
	}

	DX11_VertexBuffer::~DX11_VertexBuffer()
	{
	}

	void DX11_VertexBuffer::Bind() const
	{
		UINT stride = mLayout.GetStride();
		UINT offset = 0;

		dynamic_cast<DX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->IASetVertexBuffers(0, 1, mBuffer.GetAddressOf(), &stride, &offset);
	}

	void DX11_VertexBuffer::Unbind() const
	{
	}

	void DX11_VertexBuffer::SetData(const void* data, uint32_t size)
	{
	}

	/////////////////////////////////////////////////////
	///////////INDEX BUFFER//////////////////////////////
	/////////////////////////////////////////////////////

	DX11_IndexBuffer::DX11_IndexBuffer(uint32_t* indices, uint32_t count)
	{
		mCount = count;

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(uint32_t) * mCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData;
		ZeroMemory(&indexBufferData, sizeof(indexBufferData));
		indexBufferData.pSysMem = indices;

		HRESULT hr = dynamic_cast<DX11_Renderer*>(Renderer::GetAPI())->GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, mBuffer.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create index buffer: " + ResultInfo(hr) + "\n");
	}

	DX11_IndexBuffer::~DX11_IndexBuffer()
	{
	}

	void DX11_IndexBuffer::Bind()
	{
		dynamic_cast<DX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->IASetIndexBuffer(mBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11_IndexBuffer::Unbind()
	{
	}

	/////////////////////////////////////////////////////
	///////////VERTEX DATA///////////////////////////////
	/////////////////////////////////////////////////////

	DX11_VertexResources::DX11_VertexResources()
	{
	}

	DX11_VertexResources::~DX11_VertexResources()
	{
	}

	void DX11_VertexResources::Bind() const
	{
		for (auto& vertexBuffer : mVertexBuffers)
		{
			vertexBuffer->Bind();
		}
		mIndexBuffer->Bind();
	}

	void DX11_VertexResources::Unbind() const
	{
	}

	void DX11_VertexResources::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		mVertexBuffers.push_back(vertexBuffer);	
	}

	void DX11_VertexResources::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{		
		mIndexBuffer = indexBuffer;
	}
}


