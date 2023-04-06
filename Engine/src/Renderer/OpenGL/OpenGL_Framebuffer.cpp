#include "aio_pch.h"
#include "OpenGL_Framebuffer.h"


namespace Alexio
{
	OpenGL_Framebuffer::OpenGL_Framebuffer()
	{
	}

	OpenGL_Framebuffer::OpenGL_Framebuffer(const FramebufferSpecification& spec)
	{
		mSpecification = spec;
	}

	OpenGL_Framebuffer::~OpenGL_Framebuffer()
	{
		glDeleteFramebuffers(1, &mID);
	}

	void OpenGL_Framebuffer::Invalidate()
	{
		glCreateFramebuffers(1, &mID);
		glBindFramebuffer(GL_FRAMEBUFFER, mID);

		glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
		glBindTexture(GL_TEXTURE_2D, mColorAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, mSpecification.width, mSpecification.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mID, 0);

		AIO_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGL_Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
	}

	void OpenGL_Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

