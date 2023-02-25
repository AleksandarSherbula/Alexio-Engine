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
	class DirectX11_Renderer : public RendererAPI 
	{
	public:
		DirectX11_Renderer();

		void Initialize() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void Draw(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexResources>& vertexResources = nullptr) override;
		void ClearColor(float r, float g, float b, float a) override;
		void SwapBuffer() override;

		// Getters
		inline Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const { return mDevice; }
		inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return mDeviceContext; }
		inline Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() const { return mSwapChain; }
		inline std::string GetName() const override { return "DirectX 11"; }

		void CreateRenderTarget();
		void CleanRenderTarget();
	private:
		// States
		void GetAdapters();

		void ImGuiBackendInit() override;
		void ImGuiBackendBegin() override;
		void ImGuiBackendUpdate() override;
		void ImGuiBackendShutDown() override;
	private:
		static DirectX11_Renderer* sInstance;
	private:
		std::vector<AdapterData> mAdapters;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
	};

	std::string ResultInfo(HRESULT hr);
}