#pragma once

#include "Renderer/Shader.h"

namespace Alexio
{
	class DirectX11_Shader : public Shader
	{
	public:
		DirectX11_Shader(const std::string& name);
		DirectX11_Shader(const std::string& name, const std::string& filepath);
		DirectX11_Shader(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc);
		~DirectX11_Shader();

		void Compile() override;
		
		void Bind() const override;
		void Unbind() const override;
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mVertexShaderBuffer;

		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mPixelShaderBuffer;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> mVertexLayout;
	};
}