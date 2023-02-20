#pragma once

#include "Renderer/Shader.h"

namespace Alexio
{
	class DirectX11_Shader : public Shader
	{
	public:
		DirectX11_Shader(const std::string& vertexSrc, const std::string& pixelSrc);
		~DirectX11_Shader();

		void Bind() const override;
		void Unbind() const override;
	private:
		uint32_t mID;
	};
}