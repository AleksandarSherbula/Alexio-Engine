#pragma once

#include <string>

#include "Math/Math.h"
#include "Renderer/Buffer.h"
#include "Buffer.h"

namespace Alexio
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		static Ref<Shader> Create(const std::string& name, const Ref<VertexBuffer>& vertexBuffer);
		static Ref<Shader> Create(const std::string& name, const std::string& filepath, const Ref<VertexBuffer>& vertexBuffer);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc, const Ref<VertexBuffer>& vertexBuffer);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int32_t value) = 0;
		virtual void SetIntArray(const std::string& name, int32_t* values, uint32_t count) = 0;

		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const Vector2& value) = 0;
		virtual void SetFloat3(const std::string& name, const Vector3& value) = 0;
		virtual void SetFloat4(const std::string& name, const Vector4& value) = 0;

		virtual void SetMat3x3(const std::string& name, const glm::mat3x3& matrix) = 0;
		virtual void SetMat4x4(const std::string& name, const Mat4x4& matrix) = 0;

		inline const std::string& GetName() const { return mName; }
	protected:
		std::string mName;
	};
}