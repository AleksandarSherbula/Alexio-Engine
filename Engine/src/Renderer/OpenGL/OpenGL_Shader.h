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
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	private:
		uint32_t mID;
		std::unordered_map<GLenum, std::string> mShaderSource;
	};
}