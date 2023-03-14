#pragma once

#include "Renderer/Texture.h"
#if defined(AIO_API_DX11)

namespace Alexio
{
	class DX11_Texture : public Texture
	{
	public:
		DX11_Texture(uint32_t width, uint32_t height);
		DX11_Texture(const std::string& filepath);

		void SetData(const void* data, uint32_t width, uint32_t height) override;

		void Bind(uint32_t slot) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureResource;
	};
}
#endif