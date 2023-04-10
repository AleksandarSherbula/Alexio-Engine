#pragma once

namespace Alexio
{
	struct FramebufferSpecification
	{
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t samples = 1;
	};

	class Framebuffer
	{
	public:
		virtual void Invalidate() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetColorAttachment() const = 0;

		virtual void Clear(float r, float g, float b, float a) = 0;

		inline const FramebufferSpecification& GetSpecification() const { return mSpecification; }

		virtual void OnResize(float width, float height) = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	protected:
		FramebufferSpecification mSpecification;
	};
}