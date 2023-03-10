#pragma once

#include "Renderer/Shader.h"

namespace Alexio
{
	class DX11_Shader : public Shader
	{
	public:
		DX11_Shader(const std::string& name);
		DX11_Shader(const std::string& name, const std::string& filepath);
		DX11_Shader(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc);
		~DX11_Shader();

		void Compile() override;
		
		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int32_t value) override;
		void SetIntArray(const std::string& name, int32_t* values, uint32_t count) override;
		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const Vector2f& value) override;
		void SetFloat3(const std::string& name, const Vector3f& value) override;
		void SetFloat4(const std::string& name, const Vector4f& value) override;

		void SetMat3x3(const std::string& name, const glm::mat3x3& matrix) override {}
		void SetMat4x4(const std::string& name, const glm::mat4x4& matrix) override {}
	private:
		std::string mVertexSource;
		std::string mPixelSource;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mVertexShaderBuffer;

		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> mPixelShaderBuffer;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> mVertexLayout;
	};
}