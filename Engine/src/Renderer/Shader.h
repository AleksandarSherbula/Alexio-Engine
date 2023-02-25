#pragma once

#include <string>
#include <memory>

#include "Renderer/Buffer.h"

namespace Alexio
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		static std::shared_ptr<Shader> Create(const std::string& name);
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& filepath);
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc);

		inline const std::string& GetName() const { return mName; }
		inline void SetVertexResources(const std::shared_ptr<VertexResources>& vr) { mVertexResources = vr; }
		inline VertexResources* GetVertexResources() const { return mVertexResources.get(); }
		virtual void Compile() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	protected:
		std::string mName;
		std::string mVertexSource;
		std::string mPixelSource;

		std::shared_ptr<VertexResources> mVertexResources;
	};
}