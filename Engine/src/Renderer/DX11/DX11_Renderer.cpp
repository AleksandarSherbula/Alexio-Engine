#include "aio_pch.h"
#include "DX11_Renderer.h"
#include "Window/Win32_Window.h"

#include "Alexio/Engine.h"
#include "DX11_Buffer.h"

#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

namespace Alexio
{
	DX11_Renderer* DX11_Renderer::sInstance = nullptr;

	DX11_Renderer::DX11_Renderer()
	{
		mWindow = nullptr;
		AIO_ASSERT(!sInstance, "OpenGL API object was already been made");
		sInstance = this;

		GetAdapters();
	}

	void DX11_Renderer::Initialize()
	{
		if (mAdapters.empty())
			AIO_LOG_ERROR("NO IDXGI Adapter found");

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(scd));
		scd.BufferCount = 1;
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = (HWND)mWindow->GetHandle();
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

		UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef AIO_DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // AIO_DEBUG

		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, 
			flags, 
			featureLevelArray, 
			2,    
			D3D11_SDK_VERSION,
			&scd, 
			mSwapChain.GetAddressOf(),
			mDevice.GetAddressOf(),
			&featureLevel, 
			mDeviceContext.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create device and swapchain: " + ResultInfo(hr) + "\n");

		CreateRenderTarget();

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (FLOAT)Engine::GetInstance()->GetWindow()->GetWidth();
		viewport.Height = (FLOAT)Engine::GetInstance()->GetWindow()->GetHeight();

		mDeviceContext->RSSetViewports(1, &viewport);

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;

		hr = mDevice->CreateRasterizerState(&rasterizerDesc, mRasterizerState.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create rasterizer state: " + ResultInfo(hr) + "\n");

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));

		//D3D11_RENDER_TARGET_BLEND_DESC rtbd;
		//ZeroMemory(&rtbd, sizeof(rtbd));

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;


		hr = mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create blend state: " + ResultInfo(hr) + "\n");

		AIO_LOG_INFO("DirectX 11 Initialized");
	}

	void DX11_Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		if (mDevice != nullptr)
		{
			CleanRenderTarget();
			mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)height;

		mDeviceContext->RSSetViewports(1, &viewport);
	}

	void DX11_Renderer::Draw(const Ref<Shader>& shader, const Ref<VertexResources>& vertexData)
	{
		mDeviceContext->DrawIndexed(shader->GetVertexResources()->GetIndexBuffer()->GetCount(), 0, 0);		
	}

	void DX11_Renderer::ClearColor(float r, float g, float b, float a)
	{
		FLOAT bgColor[] = { r, g, b, a };
		mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), NULL);
		mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);
		mDeviceContext->RSSetState(mRasterizerState.Get());
		mDeviceContext->OMSetBlendState(mBlendState.Get(), NULL, 0xffffff);
	}

	void DX11_Renderer::SwapBuffer()
	{		
		mSwapChain->Present((UINT)mVSync, 0);
	}

	void DX11_Renderer::ImGuiBackendInit()
	{
		HWND hwnd = (HWND)Engine::GetInstance()->GetWindow()->GetHandle();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(mDevice.Get(), mDeviceContext.Get());
	}

	void DX11_Renderer::ImGuiBackendBegin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
	}

	void DX11_Renderer::ImGuiBackendUpdate()
	{		
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void DX11_Renderer::ImGuiBackendShutDown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}

	void DX11_Renderer::CreateRenderTarget()
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(backBuffer.GetAddressOf()));
		mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
	}

	void DX11_Renderer::CleanRenderTarget()
	{
		if (mRenderTargetView)
			mRenderTargetView->Release();
	}

	void DX11_Renderer::GetAdapters()
	{
		if (!mAdapters.empty())
			mAdapters.clear();

		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

		// Create a DXGIFactory object
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());
		AIO_ASSERT(!FAILED(hr), "Failed to create DXGIFactory for enumerating adapters: " + ResultInfo(hr) + "\n");

		IDXGIAdapter* pAdapter;
		UINT index = 0;
		while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
		{
			mAdapters.push_back({ pAdapter });
			mAdapters[index].ptr->GetDesc(&mAdapters[index].description);
			index += 1;
		}
	}
}