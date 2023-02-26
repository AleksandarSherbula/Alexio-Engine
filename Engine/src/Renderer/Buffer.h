#pragma once

#include <memory>

#include "Alexio/Log.h"

namespace Alexio
{
	enum class ShaderDataType
	{
		None, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{		
		case ShaderDataType::Float:  return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3:   return 4 * 3 * 3;
		case ShaderDataType::Mat4:   return 4 * 4 * 4;
		case ShaderDataType::Int:    return 4;
		case ShaderDataType::Int2:   return 4 * 2;
		case ShaderDataType::Int3:   return 4 * 3;
		case ShaderDataType::Int4:   return 4 * 4;
		case ShaderDataType::Bool:   return 1;
		}

		AIO_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
		{
			this->name = name;
			this->type = type;
			this->normalized = normalized;
			size = ShaderDataTypeSize(type);
			offset = 0;
		}

		uint32_t GetComponentCount() const
		{
			switch (type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3; // 3* float3
			case ShaderDataType::Mat4:    return 4 * 4; // 4* float4
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			AIO_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
		{
			mElements = elements;
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return mStride; }
		inline const std::vector<BufferElement>& GetElements() const { return mElements; }

		std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
		std::vector<BufferElement>::iterator end() { return mElements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			mStride = 0;
			for (auto& element : mElements)
			{
				element.offset = offset;
				offset += element.size;
				mStride += element.size;
			}
		}
	private:
		std::vector<BufferElement> mElements;
		uint32_t mStride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		void SetLayout(const BufferLayout& layout) { mLayout = layout; }
		const BufferLayout& GetLayout() const { return mLayout; }

		static std::shared_ptr<VertexBuffer> Create(uint32_t size);
		static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
	protected:
		BufferLayout mLayout;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		inline uint32_t GetCount() const { return mCount; }
		static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	protected:
		uint32_t mCount;
	};

	class VertexResources
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		inline std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() { return mVertexBuffers; }
		inline std::shared_ptr<IndexBuffer> GetIndexBuffer() { return mIndexBuffer; }

		static std::shared_ptr<VertexResources> Create();

	protected:
		std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
	};
}