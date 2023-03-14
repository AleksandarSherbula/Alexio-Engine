#pragma once

#include "Renderer/Texture.h"

#if defined(AIO_API_OPENGL)
namespace Alexio
{
	class OpenGL_Texture : public Texture
	{
	public:
		OpenGL_Texture(uint32_t width, uint32_t height);
		OpenGL_Texture(const std::string& filepath);

		~OpenGL_Texture();

		void SetData(const void* data, uint32_t width, uint32_t height) override;

		void Bind(uint32_t slot) override;
	private:
		uint32_t mID;
		GLenum mInternalFormat, mDataFormat;
	};
}
#endif