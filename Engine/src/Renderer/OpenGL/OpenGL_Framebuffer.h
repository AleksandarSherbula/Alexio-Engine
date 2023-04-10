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

		void Clear(float r, float g, float b, float a) override;

		inline void* GetColorAttachment() const override { return (void*)mColorAttachment; }

		void Invalidate() override;
		void OnResize(float width, float height) override;
	private:
		uint32_t mID;
		uint32_t mColorAttachment;
	};
}