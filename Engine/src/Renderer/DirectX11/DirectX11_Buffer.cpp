#include "aio_pch.h"
#include "DirectX11_Buffer.h"
#include "Alexio/Renderer.h"
#include "DirectX11_Renderer.h"

namespace Alexio
{
	DirectX11_VertexBuffer::DirectX11_VertexBuffer(uint32_t size)
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

		HRESULT hr = dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, mBuffer.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer: " + ResultInfo(hr) + "\n");
	}

	DirectX11_VertexBuffer::DirectX11_VertexBuffer(float* vertices, uint32_t size)
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

		HRESULT hr = dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, mBuffer.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer: " + ResultInfo(hr) + "\n");
	}

	DirectX11_VertexBuffer::~DirectX11_VertexBuffer()
	{
	}

	void DirectX11_VertexBuffer::Bind() const
	{
		UINT stride = mLayout.GetStride();
		UINT offset = 0;

		dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->IASetVertexBuffers(0, 1, mBuffer.GetAddressOf(), &stride, &offset);
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

		HRESULT hr = dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, mBuffer.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create index buffer: " + ResultInfo(hr) + "\n");
	}

	DirectX11_IndexBuffer::~DirectX11_IndexBuffer()
	{
	}

	void DirectX11_IndexBuffer::Bind()
	{
		dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->IASetIndexBuffer(mBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DirectX11_IndexBuffer::Unbind()
	{
	}

	/////////////////////////////////////////////////////
	///////////VERTEX DATA///////////////////////////////
	/////////////////////////////////////////////////////

	DirectX11_VertexResources::DirectX11_VertexResources()
	{
	}

	DirectX11_VertexResources::~DirectX11_VertexResources()
	{
	}

	void DirectX11_VertexResources::Bind() const
	{
		for (auto& vertexBuffer : mVertexBuffers)
		{
			vertexBuffer->Bind();
		}
		mIndexBuffer->Bind();
	}

	void DirectX11_VertexResources::Unbind() const
	{
	}

	void DirectX11_VertexResources::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		mVertexBuffers.push_back(vertexBuffer);	
	}

	void DirectX11_VertexResources::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{		
		mIndexBuffer = indexBuffer;
	}
}


