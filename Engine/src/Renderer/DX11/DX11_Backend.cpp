#include "aio_pch.h"

#if defined(AIO_API_DX11)
#include "DX11_Backend.h"
#include "DX11_Buffer.h"
#include "DX11_Shader.h"

#include "Window/Win32_Window.h"
#include "Alexio/Engine.h"

#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

namespace Alexio
{
	DX11_Backend* DX11_Backend::sInstance = nullptr;

	DX11_Backend::DX11_Backend()
	{
		AIO_ASSERT(!sInstance, "DirectX11 backend has already been made");
		sInstance = this;
	}

	DX11_Backend::~DX11_Backend()
	{
	}

	void DX11_Backend::Initialize()
	{
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(scd));
		scd.BufferCount = 2;
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = (HWND)Engine::Get()->GetWindow()->GetHandle();
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[4] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };

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
		viewport.Width = Engine::Get()->GetWindow()->GetCallbackData().width;
		viewport.Height = Engine::Get()->GetWindow()->GetCallbackData().height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		mDeviceContext->RSSetViewports(1, &viewport);

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;

		hr = mDevice->CreateRasterizerState(&rasterizerDesc, mRasterizerState.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create rasterizer state: " + ResultInfo(hr) + "\n");

		mDeviceContext->RSSetState(mRasterizerState.Get());

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		hr = mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
		AIO_ASSERT(SUCCEEDED(hr), "Failed to create blend state: " + ResultInfo(hr) + "\n");

		mDeviceContext->OMSetBlendState(mBlendState.Get(), nullptr, 0xffffff);

		mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr);

		AIO_LOG_INFO("DirectX 11 Initialized");
	}

	void DX11_Backend::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		CleanRenderTarget();
		mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		CreateRenderTarget();

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)height;

		mDeviceContext->RSSetViewports(1, &viewport);
	}

	void DX11_Backend::Draw(uint32_t vertexCount)
	{
		mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		mDeviceContext->Draw(vertexCount, 0);
	}

	void DX11_Backend::DrawIndexed(uint32_t indexCount)
	{
		mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mDeviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void DX11_Backend::Clear(float r, float g, float b, float a)
	{
		FLOAT bgColor[] = { r, g, b, a };
		mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);
		mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr);
	}

	void DX11_Backend::SwapBuffer()
	{		
		mSwapChain->Present((UINT)mVSync, 0);
	}

	void DX11_Backend::ImGuiBackendInit()
	{
		HWND hwnd = (HWND)Engine::Get()->GetWindow()->GetHandle();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(mDevice.Get(), mDeviceContext.Get());
	}

	void DX11_Backend::ImGuiBackendBegin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void DX11_Backend::ImGuiBackendUpdate()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void DX11_Backend::ImGuiBackendShutDown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}

	void DX11_Backend::CreateRenderTarget()
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(backBuffer.GetAddressOf()));
		mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
	}

	void DX11_Backend::CleanRenderTarget()
	{
		if (mRenderTargetView)
			mRenderTargetView->Release();
	}

	void DX11_Backend::SetRenderTarget()
	{
		mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr);
	}
}
#endif