#include "aio_pch.h"
#include "DirectX11_Renderer.h"
#include "Window/Win32_Window.h"

namespace Alexio
{
	Renderer_DirectX11* Renderer_DirectX11::sInstance = nullptr;

	Renderer_DirectX11::Renderer_DirectX11()
	{
		mWindow = nullptr;
		AIO_ASSERT(!sInstance, "OpenGL API object was already been made");
		sInstance = this;

		GetAdapters();		
	}

	void Renderer_DirectX11::Initialize()
	{
		if (mAdapters.empty())
			AIO_LOG_ERROR("NO IDXGI Adapter found");

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = mWindow->GetWidth();
		scd.BufferDesc.Height = mWindow->GetHeight();
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = (HWND)mWindow->GetHandle();
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(mAdapters[0].ptr, // IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL, // FOR SOFTWARE DRIVER TYPE
			NULL, // FLAGS FOR RUNTIME LAYERS
			NULL, // FEATURE LEVELS ARRAY
			0,    // # OF FEATURE LEVELS IN ARRAY
			D3D11_SDK_VERSION,
			&scd, // SwapChain description
			mSwapChain.GetAddressOf(),
			mDevice.GetAddressOf(),
			NULL, // Supported feature level
			mDeviceContext.GetAddressOf());
		AIO_ASSERT(!FAILED(hr), "Failed to create device and swapchain: " + ResultInfo(hr) + "\n");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(backBuffer.GetAddressOf()));
		AIO_ASSERT(!FAILED(hr), "GetBuffer failed: " + ResultInfo(hr) + "\n");

		hr = mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
		AIO_ASSERT(!FAILED(hr), "Failed to create render target view: " + ResultInfo(hr) + "\n");

		mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), NULL);

		AIO_LOG_INFO("DirectX 11 Initialized");
	}

	void Renderer_DirectX11::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
	}

	void Renderer_DirectX11::Draw()
	{
	}

	void Renderer_DirectX11::ClearColor(float r, float g, float b, float a)
	{
		FLOAT bgColor[] = { r, g, b, a };
		mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);
	}

	void Renderer_DirectX11::SwapBuffer()
	{
		mSwapChain->Present((UINT)mVSync, NULL);
	}

	void Renderer_DirectX11::GetAdapters()
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