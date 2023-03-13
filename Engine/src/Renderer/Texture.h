#pragma once

namespace Alexio
{
	class Texture
	{
	public:
		virtual ~Texture() {}

		inline uint32_t GetWidth() { return mWidth; }
		inline uint32_t GetHeight() { return mHeight; }

		virtual void Bind(uint32_t slot) = 0;

		virtual void SetData(const void* data, uint32_t width, uint32_t height) = 0;

		static Ref<Texture> Create(uint32_t width, uint32_t height);
		static Ref<Texture> Create(const std::string& filepath);
	protected:
		uint32_t mWidth, mHeight;
	};
}