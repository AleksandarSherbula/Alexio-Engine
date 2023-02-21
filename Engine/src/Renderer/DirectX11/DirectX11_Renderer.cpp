#include "aio_pch.h"
#include "DirectX11_Renderer.h"
#include "Window/Win32_Window.h"

#include "Alexio/Engine.h"

#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

namespace Alexio
{
	DirectX11_Renderer* DirectX11_Renderer::sInstance = nullptr;

	DirectX11_Renderer::DirectX11_Renderer()
	{
		mWindow = nullptr;
		AIO_ASSERT(!sInstance, "OpenGL API object was already been made");
		sInstance = this;

		GetAdapters();
	}

	void DirectX11_Renderer::Initialize()
	{
		if (mAdapters.empty())
			AIO_LOG_ERROR("NO IDXGI Adapter found");

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
		hr = D3D11CreateDeviceAndSwapChain(mAdapters[0].ptr, // IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL, // FOR SOFTWARE DRIVER TYPE
			flags, // FLAGS FOR RUNTIME LAYERS
			featureLevelArray, // FEATURE LEVELS ARRAY
			2,    // # OF FEATURE LEVELS IN ARRAY
			D3D11_SDK_VERSION,
			&scd, // SwapChain description
			mSwapChain.GetAddressOf(),
			mDevice.GetAddressOf(),
			&featureLevel, // Supported feature level
			mDeviceContext.GetAddressOf());
		AIO_ASSERT(!FAILED(hr), "Failed to create device and swapchain: " + ResultInfo(hr) + "\n");

		CreateRenderTarget();

		AIO_LOG_INFO("DirectX 11 Initialized");
	}

	void DirectX11_Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		CleanRenderTarget();
		mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		CreateRenderTarget();
	}

	void DirectX11_Renderer::Draw(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexData>& vertexData)
	{
	}

	void DirectX11_Renderer::ClearColor(float r, float g, float b, float a)
	{
		FLOAT bgColor[] = { r, g, b, a };
		mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);
	}

	void DirectX11_Renderer::SwapBuffer()
	{
		mSwapChain->Present((UINT)mVSync, 0);
	}

	void DirectX11_Renderer::ImGuiBackendInit()
	{
		HWND hwnd = (HWND)Engine::GetInstance()->GetWindow()->GetHandle();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(mDevice.Get(), mDeviceContext.Get());
	}

	void DirectX11_Renderer::ImGuiBackendBegin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
	}

	void DirectX11_Renderer::ImGuiBackendUpdate()
	{
		mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void DirectX11_Renderer::ImGuiBackendShutDown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}

	void DirectX11_Renderer::CreateRenderTarget()
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(backBuffer.GetAddressOf()));
		mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
	}

	void DirectX11_Renderer::CleanRenderTarget()
	{
		if (mRenderTargetView)
			mRenderTargetView->Release();
	}

	void DirectX11_Renderer::GetAdapters()
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