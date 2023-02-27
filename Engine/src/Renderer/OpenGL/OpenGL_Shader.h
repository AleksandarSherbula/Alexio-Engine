#pragma once

#include "Renderer/Shader.h"

namespace Alexio
{
	class OpenGL_Shader : public Shader
	{
	public:
		OpenGL_Shader(const std::string& name);
		OpenGL_Shader(const std::string& name, const std::string& filepath);
		OpenGL_Shader(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc);
		~OpenGL_Shader();

		void Compile() override;

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int32_t value) override;
		void SetIntArray(const std::string& name, int32_t* values, uint32_t count) override;
		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const Vector2f& value) override;
		void SetFloat3(const std::string& name, const Vector3f& value) override;
		void SetFloat4(const std::string& name, const Vector4f& value) override;

		//void SetMat3(const std::string& name, const glm::mat3& matrix) override;
		//void SetMat4(const std::string& name, const glm::mat4& matrix) override;
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	private:
		uint32_t mID;
		std::unordered_map<GLenum, std::string> mShaderSource;
	};
}