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
		Invalidate();
	}

	OpenGL_Framebuffer::~OpenGL_Framebuffer()
	{
		glDeleteTextures(1, &mColorAttachment);
		glDeleteFramebuffers(1, &mID);
	}

	void OpenGL_Framebuffer::Invalidate()
	{
		glCreateFramebuffers(1, &mID);
		glBindFramebuffer(GL_FRAMEBUFFER, mID);

		glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
		glBindTexture(GL_TEXTURE_2D, mColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mSpecification.width, mSpecification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

		AIO_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGL_Framebuffer::OnResize(float width, float height)
	{
		mSpecification.width = width;
		mSpecification.height = height;

		if (mID)
		{
			glDeleteTextures(1, &mColorAttachment);
			glDeleteFramebuffers(1, &mID);
		}

		Invalidate();
	}

	void OpenGL_Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
		glViewport(0, 0, mSpecification.width, mSpecification.height);
	}

	void OpenGL_Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGL_Framebuffer::Clear(float r, float g, float b, float a)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(r, g, b, a);
	}
}

