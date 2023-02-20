#pragma once

#include <string>

namespace Alexio
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		static std::unique_ptr<Shader> Create(const std::string& vertexSrc, const std::string& pixelSrc);
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	private:
		uint32_t mID;
	};
}