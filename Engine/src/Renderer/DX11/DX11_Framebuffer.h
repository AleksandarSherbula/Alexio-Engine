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

		inline void* GetColorAttachment() const override 
		{ 
			return mColorAttachmentSRV.Get(); 
		}

		void Invalidate() override;
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mFrameBufferRTV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorAttachmentSRV;
	};
}