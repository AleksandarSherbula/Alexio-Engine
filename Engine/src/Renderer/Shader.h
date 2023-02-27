#pragma once

#include <string>

#include "Alexio/Math.h"
#include "Renderer/Buffer.h"

namespace Alexio
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		static Ref<Shader> Create(const std::string& name);
		static Ref<Shader> Create(const std::string& name, const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc);

		virtual void Compile() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int32_t value) = 0;
		virtual void SetIntArray(const std::string& name, int32_t* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const Vector2f& value) = 0;
		virtual void SetFloat3(const std::string& name, const Vector3f& value) = 0;
		virtual void SetFloat4(const std::string& name, const Vector4f& value) = 0;
																	 
		//virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
		//virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;

		inline const std::string& GetName() const { return mName; }

		inline void SetVertexResources(const Ref<VertexResources>& vr) { mVertexResources = vr; }
		inline VertexResources* GetVertexResources() const { return mVertexResources.get(); }		
	protected:
		std::string mName;
		Ref<VertexResources> mVertexResources;
	};
}