#pragma once

#include "Renderer/RendererAPI.h"

#include <d3d11.h>
#include <wrl/client.h>

struct AdapterData
{
	IDXGIAdapter* ptr = nullptr;
	DXGI_ADAPTER_DESC description;
};

namespace Alexio
{
	class Renderer_DirectX11 : public RendererAPI 
	{
	public:
		Renderer_DirectX11();

		void Initialize() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void Draw() override;
		void ClearColor(float r, float g, float b, float a) override;
		void SwapBuffer() override;

		// Getters
		inline static Renderer_DirectX11* GetInstance() { return sInstance; }
		inline ID3D11Device* GetDevice() { return mDevice.Get(); }
		inline ID3D11DeviceContext* GetDeviceContext() { return mDeviceContext.Get(); }
	private:
		// States
		void GetAdapters();
	private:
		static Renderer_DirectX11* sInstance;
	private:
		std::vector<AdapterData> mAdapters;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	};
}