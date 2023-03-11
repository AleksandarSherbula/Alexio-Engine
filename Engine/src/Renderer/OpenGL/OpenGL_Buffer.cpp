#include "aio_pch.h"
#include "OpenGL_Buffer.h"

namespace Alexio
{
	OpenGL_VertexBuffer::OpenGL_VertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &mID);
		glBindBuffer(GL_ARRAY_BUFFER, mID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGL_VertexBuffer::OpenGL_VertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &mID);
		glBindBuffer(GL_ARRAY_BUFFER, mID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	OpenGL_VertexBuffer::~OpenGL_VertexBuffer()
	{
		glDeleteBuffers(1, &mID);
	}

	void OpenGL_VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mID);
	}

	void OpenGL_VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGL_VertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/////////////////////////////////////////////////////
	///////////INDEX BUFFER//////////////////////////////
	/////////////////////////////////////////////////////

	OpenGL_IndexBuffer::OpenGL_IndexBuffer(uint32_t* indices, uint32_t count)
	{
		mCount = count;

		glCreateBuffers(1, &mID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGL_IndexBuffer::~OpenGL_IndexBuffer()
	{
		glDeleteBuffers(1, &mID);
	}

	void OpenGL_IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
	}

	void OpenGL_IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	/////////////////////////////////////////////////////
	///////////VERTEX ARRAY//////////////////////////////
	/////////////////////////////////////////////////////

	static GLenum ShaderDataTypeSizeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		AIO_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &mID);
		glBindVertexArray(mID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &mID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(mID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		AIO_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout");

		glBindVertexArray(mID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		auto& layout = vertexBuffer->GetLayout();
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeSizeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.offset);
			index++;
		}

		mVertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(mID);

		indexBuffer->Bind();
		mIndexBuffer = indexBuffer;
	}

	UniformBuffer::UniformBuffer(uint32_t block_size, uint32_t slot)
	{
		glCreateBuffers(1, &mID);
		glNamedBufferData(mID, block_size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, mID);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &mID);
	}

	void UniformBuffer::SetData(const void* data, uint32_t data_size)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, data_size, data);
	}

	void UniformBuffer::Bind(uint32_t binding)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mID);
	}
}