#include "aio_pch.h"
#include "DX11_Framebuffer.h"

#include "Renderer/Renderer.h"
#include "DX11_Backend.h"

namespace Alexio
{
	DX11_Framebuffer::DX11_Framebuffer()
	{
	}
	DX11_Framebuffer::DX11_Framebuffer(const FramebufferSpecification& spec)
	{
		mSpecification = spec;
		Invalidate();
	}

	DX11_Framebuffer::~DX11_Framebuffer()
	{

	}
	void DX11_Framebuffer::Bind() const
	{
		FLOAT bgColor[] = { 0.0f, 0.8f, 1.0f, 1.0f };
		AIO_DX11_BACKEND->GetDeviceContext()->OMSetRenderTargets(1, mFrameBufferRTV.GetAddressOf(), nullptr);
		AIO_DX11_BACKEND->GetDeviceContext()->ClearRenderTargetView(mFrameBufferRTV.Get(), bgColor);
	}

	void DX11_Framebuffer::Unbind() const
	{
		//AIO_DX11_BACKEND->GetDeviceContext()->OMSetBlendState(AIO_DX11_BACKEND->GetBlendState(), nullptr, 0xffffff);
	}

	void DX11_Framebuffer::Invalidate()
	{
		// Create Framebuffer
		D3D11_TEXTURE2D_DESC texDesc = {};
		ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
		texDesc.Width = mSpecification.width;
		texDesc.Height = mSpecification.height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		DXGI_FORMAT format = texDesc.Format;

		HRESULT hr = AIO_DX11_BACKEND->GetDevice()->CreateTexture2D(&texDesc, nullptr, mBackBuffer.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create back buffer: " + ResultInfo(hr) + "\n");

		// Create the render target view for the color attachment
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = texDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	
		hr = AIO_DX11_BACKEND->GetDevice()->CreateRenderTargetView(mBackBuffer.Get(), &rtvDesc, mFrameBufferRTV.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create render target view: " + ResultInfo(hr) + "\n");

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = texDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		hr = AIO_DX11_BACKEND->GetDevice()->CreateShaderResourceView(mBackBuffer.Get(), &shaderResourceViewDesc, mColorAttachmentSRV.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create shader resource view: " + ResultInfo(hr) + "\n");
	}
}