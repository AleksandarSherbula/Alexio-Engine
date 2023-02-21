#pragma once

#include "Renderer/Shader.h"

namespace Alexio
{
	class OpenGL_Shader : public Shader
	{
	public:
		OpenGL_Shader(const std::string& vertexSrc, const std::string& pixelSrc);
		~OpenGL_Shader();


		void Initialize() override;
		void Bind() const override;
		void Unbind() const override;
		
	private:
		uint32_t mID;
	};
}