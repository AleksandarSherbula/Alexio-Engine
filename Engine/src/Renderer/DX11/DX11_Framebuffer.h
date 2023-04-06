#pragma once

#include "Renderer/Framebuffer.h"

namespace Alexio
{
	class DX11_Framebuffer : public Framebuffer
	{
	public:
		DX11_Framebuffer();
		DX11_Framebuffer(const FramebufferSpecification& spec);
		~DX11_Framebuffer();

		void Bind() const override;
		void Unbind() const override;

		inline void* GetColorAttachment() const override { return nullptr; }

		void Invalidate() override;
	};
}