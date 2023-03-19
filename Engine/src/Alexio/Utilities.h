#pragma once

#include <string>
#include <memory>

#define BIT(x) (1 << x)

#define AIO_API_OPENGL
#define AIO_API_DX11

#if !defined (AIO_PLATFORM_WINDOWS)
#error Alexio Engine currently only runs on Windows
#endif

inline std::wstring StringToWide(const std::string& str)
{
	std::wstring wide_string(str.begin(), str.end());
	return wide_string;
}

inline std::string WideToString(const std::wstring& wstr)
{
	std::string str(wstr.begin(), wstr.end());
	return str;
}

enum GraphicsAPI
{
	Null,
	OpenGL,
	DirectX11
};

namespace Alexio
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#if defined (AIO_API_OPENGL)
    #define AIO_OPENGL_RENDERER dynamic_cast<OpenGL_Backend*>(Renderer::GetBackend())
#endif
#if defined (AIO_API_DX11)
    #define AIO_DX11_RENDERER  dynamic_cast<DX11_Backend*>(Renderer::GetBackend())
#endif
#if defined (AIO_API_DX11) && !defined(AIO_PLATFORM_WINDOWS)
    #error  DirectX11 is only supported on Windows
#endif