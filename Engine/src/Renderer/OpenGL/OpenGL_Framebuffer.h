#pragma once

#include "Renderer/Framebuffer.h"

namespace Alexio
{
	class OpenGL_Framebuffer : public Framebuffer
	{
	public:
		OpenGL_Framebuffer();
		OpenGL_Framebuffer(const FramebufferSpecification& spec);
		~OpenGL_Framebuffer();

		void Bind() const override;
		void Unbind() const override;

		inline void* GetColorAttachment() const override { return (void*)mColorAttachment; }

		void Invalidate() override;
	private:
		uint32_t mID;
		uint32_t mColorAttachment;
	};
}