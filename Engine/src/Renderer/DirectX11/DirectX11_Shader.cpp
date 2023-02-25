#include "aio_pch.h"
#include "DirectX11_Shader.h"

#include "Alexio/Renderer.h"
#include "Renderer/DirectX11/DirectX11_Renderer.h"

namespace Alexio
{
	static DXGI_FORMAT ShaderDataTypeSizeToDX11BaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Int:      return DXGI_FORMAT_R32_SINT;
		case ShaderDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
		}

		AIO_ASSERT(false, "Unknown ShaderDataType!");
		return DXGI_FORMAT_UNKNOWN;
	}


	DirectX11_Shader::DirectX11_Shader(const std::string& name)
	{
		mName = name;
		std::string filepath = "../Game/res/shaders/DX11/" + name + ".hlsl";
		mVertexSource = filepath;
		mPixelSource = filepath;
	}

	DirectX11_Shader::DirectX11_Shader(const std::string& name, const std::string& filepath)
	{
		mName = name;
		mVertexSource = filepath;
		mPixelSource = filepath;
	}

	DirectX11_Shader::DirectX11_Shader(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc)
	{
		mName = name;
		mVertexSource = vertexSrc;
		mPixelSource = pixelSrc;
	}

	DirectX11_Shader::~DirectX11_Shader()
	{
	}

	void DirectX11_Shader::Compile()
	{
		Microsoft::WRL::ComPtr<ID3D11Device>& device = dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDevice();

		////////// VERTEX SHADER /////////////////
		ID3DBlob* vertexErrorMessage;
		HRESULT hr = D3DCompileFromFile(StringToWide(mVertexSource).c_str(), NULL, NULL, "VSMain", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS,
			0, &mVertexShaderBuffer, &vertexErrorMessage);
		AIO_ASSERT(SUCCEEDED(hr), "Failed to load shader: " + mVertexSource + "\n" + ResultInfo(hr));

		std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc;

		for (auto& vertexBuffer : mVertexResources->GetVertexBuffers())
		{
			auto& layout = vertexBuffer->GetLayout();
			for (auto& element : layout)
			{
				layoutDesc.push_back({ element.name.c_str(), 0, ShaderDataTypeSizeToDX11BaseType(element.type), 0, element.offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			}
		}

		hr = device->CreateInputLayout(layoutDesc.data(),
			layoutDesc.size(),
			mVertexShaderBuffer->GetBufferPointer(),
			mVertexShaderBuffer->GetBufferSize(),
			mVertexLayout.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create vertex layout: " + mVertexSource + "\n" + ResultInfo(hr));

		hr = device->CreateVertexShader(mVertexShaderBuffer->GetBufferPointer(), mVertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create vertex shader: " + mVertexSource + "\n" + ResultInfo(hr));

		////////// PIXEL SHADER /////////////////
		ID3DBlob* pixelErrorMessage;
		hr = D3DCompileFromFile(StringToWide(mPixelSource).c_str(), NULL, NULL, "PSMain", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS,
			0, mPixelShaderBuffer.GetAddressOf(), &pixelErrorMessage);
		AIO_ASSERT(SUCCEEDED(hr), "Failed to load shader: " + mPixelSource + "\n" + ResultInfo(hr));

		hr = device->CreatePixelShader(mPixelShaderBuffer->GetBufferPointer(), mPixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create pixel shader: " + mPixelSource + "\n" + ResultInfo(hr));
	}

	void DirectX11_Shader::Bind() const
	{
		dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->IASetInputLayout(mVertexLayout.Get());
		dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->VSSetShader(mVertexShader.Get(), NULL, 0);
		dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->PSSetShader(mPixelShader.Get(), NULL, 0);

		for (auto& vertexBuffer : mVertexResources->GetVertexBuffers())
		{
			vertexBuffer->Bind();
		}
		mVertexResources->GetIndexBuffer()->Bind();
	}

	void DirectX11_Shader::Unbind() const
	{
		dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->VSSetShader(nullptr, NULL, 0);
		dynamic_cast<DirectX11_Renderer*>(Renderer::GetAPI())->GetDeviceContext()->PSSetShader(nullptr, NULL, 0);
	}
}


