#pragma once

#include "Renderer/Shader.h"

#if defined(AIO_API_DX11)
namespace Alexio
{
	class DX11_Shader : public Shader
	{
	public:
		DX11_Shader(const std::string& name, const Ref<VertexBuffer>& vertexBuffer);
		DX11_Shader(const std::string& name, const std::string& filepath, const Ref<VertexBuffer>& vertexBuffer);
		DX11_Shader(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc, const Ref<VertexBuffer>& vertexBuffer);
		~DX11_Shader();

		
		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int32_t value) override {}
		void SetIntArray(const std::string& name, int32_t* values, uint32_t count) override {}
		void SetFloat(const std::string& name, float value) override {}
		void SetFloat2(const std::string& name, const Vector2& value) override {}
		void SetFloat3(const std::string& name, const Vector3& value) override {}
		void SetFloat4(const std::string& name, const Vector4& value) override {}

		void SetMat3x3(const std::string& name, const glm::mat3x3& matrix) override {}
		void SetMat4x4(const std::string& name, const Mat4x4& matrix) override {}
	private:
		void Compile(const Ref<VertexBuffer>& vertexBuffer);
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
#endif